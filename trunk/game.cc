#include <iostream>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <stack>
#include <vector>
#include <iterator>
#include <sstream>
#include "game.h"
#include "player.h"
#include "board.h"
#include "gui.h"
#include "functions.h"

using namespace std;

namespace checkers {

	Game::Game(Board& b, GUI* g) : board(b), gui(g)
	{
		black = new Player(this);
		white = new Player(this);

		state = PLAYING;

		board_count = 0;		// put elsewhere?
		move_count = 0;

		lastMove.nodes = 0;
		lastMove.time = 0;
		lastMove.value = 0;
		lastMove.extendedDepth = 0;
		lastMove.depth = 0;

		gui->printBoard(board);
		gui->printInfo(*this);
		gui->printLog();

		while(state == PLAYING)
		{
			interpretCommand(gui->input());
			gui->printInfo(*this);
		}
	}

	Game::~Game()
	{
		delete black;
		delete white;
	}

	void Game::interpretCommand(string command) {
		int size = 0;
		if((size = isMovement(command)) != 0)
		{
			vector<unsigned int> movement = parseMovement(command);
			makeMove(movement);
		}
		else if(command == "help")
		{
			gui->println("Commands: help, ai, undo, skip, quit");
		}
		else if(command == "ai")
		{
			ai();
		}
		else if(command == "undo")
		{
			if(undo())
			{
				gui->println("Reverting!");
			}
			else
			{
				gui->println("Nothing to undo!");
			}
		}
		else if(command == "skip")
		{
			board.changePlayer();
			gui->println("Skiping turn.");
		}
		else if(command == "quit")
		{
			if(gui->dialogbox("Really quit?"))
				state = QUIT;
			else
				gui->printBoard(board);
		}
		else
		{
			gui->println("Unknown command: " + command);
		}
	}

	bool Game::makeMove(vector<unsigned int>& movements)
	{
		ostringstream movestring;
		int size = movements.size();

		movestring << log2(movements[0])+1;
		for(int i = 1; i<size; i++)
		{
			movestring << "-" << log2(movements[i])+1;
		}

		int result = board.validateMove(movements);
		/////////////////////
		// result:
		// 0 Legal move.
		// -1 illegal
		// -2 more captures possible
		// -3 movements.size() < 2
		//////////////////
		if(result == 0)
		{
			if(!history.empty()) updateBoardHistory(board, history.top());
			history.push(board);

			gui->println("My move is " + movestring.str());

			if(board.getCaptureMoves(movements[0]) == 0)
			{
				board.move(movements[0], movements[1]);
			}
			else
			{
				vector<unsigned int> tmp;
				for(size_t i=1; i<movements.size(); i++)
				{
					tmp.clear();
					recursiveCapture(board, movements[i-1], movements[i], tmp);
					tmp.push_back(movements[i-1]);
					for(size_t j=tmp.size()-1; j>0; j--)
					{
						board.move(tmp[j], tmp[j-1]);
						gui->printBoard(board);
						usleep(300000);
					}
				}
			}
			board.updateKings();
			board.changePlayer();
			move_count++;

			gui->printBoard(board);

			if(board.endOfGame())
			{
				if(board.black == 0)
					state = WHITE_WON;
				else
					state = BLACK_WON;
			}
		}
		else if(result == -1) {
			gui->println("\033[31mIllegal move: " + movestring.str() + "\033[0m");
		}
		else if(result == -2) {
			gui->println("\033[31mMore captures possible: " + movestring.str() + "\033[0m");
		}
		else if(result == -3) {
			gui->println("\033[31mNot enough moves: " + movestring.str() + "\033[0m");
		}

		return result == 0;
	}

	void Game::updateBoardHistory(Board& newboard, Board& lastboard) {
		// the only time we need to add a board, is when a king moves
		if( countBits(lastboard.kings ^ newboard.kings) != 2 )
			board_count = 0;
		else if(board_count < 50)
			boards[board_count++] = newboard;
	}

	size_t Game::countHistoryMatches(Board& board) {
		size_t count = 0;
		for(size_t i = 0; i < board_count; ++i)
			if( boards[i] == board ) ++count;

		return count;
	}

	int Game::recursiveCapture(Board tmpboard, unsigned int from, unsigned int to, vector<unsigned int>& movements) {
		unsigned int moves = tmpboard.getCaptureMoves(from);
		unsigned int capture = 0x0u;
		Board test;
		while(moves != 0) {
			capture = (moves & (moves-1)) ^ moves;
			moves &= moves-1;
			test = tmpboard;

			test.move(from, capture);

			if(capture == to) {
				movements.push_back(to);
				return 0;
			}
			if(recursiveCapture(test, capture, to, movements) == 0) {
				movements.push_back(capture);
				return 0;
			}
		}
		return -1;
	}

	void Game::ai() {
		Player* p;
		board.player == BLACK ? p = black : p = white;
		gui->println("Im thinking...");
		SearchResult result = p->search();
		makeMove(result.move);
		lastMove = result;
	}

	bool Game::undo() {
		if(history.empty()) {
			return false;
		} else {
			board = history.top();
			history.pop();
			move_count--;
			gui->printBoard(board);
			return true;
		}
	}

	int Game::isMovement(string line) {
		string::iterator It = line.begin();
		string tmpstr;
		int result = 1;
		while( It != line.end()) {
			if( *It == '-') {
				result++;
				if(atoi(tmpstr.c_str()) == 0) {
					result = 0;
					break;
				}
				tmpstr = "";
			} else
				tmpstr += *It;
			It++;
		}
		if(atof(tmpstr.c_str()) == 0) {
			result = 0;
		}
		return result;
	}

	vector<unsigned int> Game::parseMovement(string line) {
		string::iterator It = line.begin();
		int i=0;
		vector<unsigned int> movement;
		string tmpstr;

		while( It != line.end()) {
			if(*It == '-') {
				movement.push_back(static_cast<unsigned int>(pow(2.0, atof(tmpstr.c_str())-1)));
				tmpstr = "";
				i++;
				It++;
				continue;
			}
			tmpstr += *It;
			It++;
		}
		movement.push_back(static_cast<unsigned int>(pow(2.0, atof(tmpstr.c_str())-1)));

		return movement;
	}
}
