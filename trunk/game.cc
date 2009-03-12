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
#include "move.h"

using namespace std;

namespace checkers {

	Game::Game(Board& board, GUI* g) : gui(g)
	{
		black = new Player(this);
		white = new Player(this);

		board_count = 0;		// put elsewhere?

		state.board = board;
		state.action = PLAYING;
		state.move_count = 0;
		state.moves_since_capture = 0;
		state.moves_since_man = 0;
		state.black_time = 0;
		state.white_time = 0;

		gui->printBoard(state.board);
		gui->gameInfo(state);
//		gui->moveInfo(last_move);
		gui->printLog();

		while(state.action == PLAYING)
		{
			interpretCommand(gui->input());

			if(state.action == DRAW)
			{
				if(gui->dialogbox("DRAW! Coninue?"))
				{
					state.action = PLAYING;
					state.moves_since_man = 0;
					state.moves_since_capture = 0;
					board_count = 0;
				}
				gui->printBoard(state.board);
			}
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
			Move move(command);
			makeMove(move);
		}
		else if(command == "help")
		{
			gui->println("Commands: help, ai, undo, skip, save, quit");
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
			state.board.changePlayer();
			gui->println("Skiping turn.");
			gui->gameInfo(state);
		}
		else if(command == "save")
		{
			gui->println("Filename:");
			std::string filename = "tests/" + gui->input();
			if(state.board.save(filename.c_str()))
				gui->println("Game saved.");
			else
				gui->println("Failed to save the game!");
		}
		else if(command == "quit")
		{
			if(gui->dialogbox("Really quit?"))
				state.action = QUIT;
			else
				gui->printBoard(state.board);
		}
		else
		{
			gui->println("Unknown command: " + command);
		}
	}

	bool Game::makeMove(Move& move)
	{
		ostringstream movestring;
		int result = move.validate(state.board);
		movestring << move;

		/////////////////////
		// result:
		// 0 Legal move.
		// -1 illegal
		// -2 more captures possible
		// -3 movements.size() < 2
		//////////////////
		if(result == 0)
		{
			Board newboard = state.board;
			if(!history.empty())
			{
				updateBoardHistory(state.board, history.top().first.board);
			}
			history.push(make_pair(state, last_move));

			gui->println("My move is " + movestring.str());

			while(move.makeNext(newboard))
			{
				usleep(300000);
				gui->printBoard(newboard);
			}
			newboard.updateKings();
			newboard.changePlayer();
			gui->printBoard(newboard);

			updateState(newboard, move);
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

	void Game::updateState(const Board& newboard, const Move& move)
	{
		state.move_count++;

		if(state.board.player == BLACK)
			state.black_time += move.time;
		else
			state.white_time += move.time;

		if(((state.board.black | state.board.white) & ~state.board.kings & move.first()) != 0)
			state.moves_since_man=0;
		else
			state.moves_since_man++;
		if(state.board.getCaptureMoves() != 0)
			state.moves_since_capture = 0;
		else
			state.moves_since_capture++;

		state.board = newboard;
		last_move = move;

		if(state.moves_since_capture > 40 && state.moves_since_man > 40)
		{
			gui->println("40 moves since last capture or man move.");
			state.action = DRAW;
		}
		if(countHistoryMatches(state.board) >= 3)
		{
			gui->println("This is the third time for this position.");
			state.action = DRAW;
		}

		if(state.board.endOfGame())
		{
			if((state.board.getCaptureMoves(state.board.black) | state.board.getMoves(state.board.black)) == 0)
				state.action = WHITE_WON;
			else
				state.action = BLACK_WON;
		}

		gui->gameInfo(state);
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

	void Game::ai() {
		Player* p;
		state.board.player == BLACK ? p = black : p = white;
		gui->println("Im thinking...");
		SearchResult result = p->search();

		gui->moveInfo(result);
		makeMove(result.move);
	}

	bool Game::undo() {
		if(history.empty()) {
			return false;
		} else {
			state = history.top().first;
			last_move = history.top().second;
			history.pop();
			gui->printBoard(state.board);
			gui->gameInfo(state);
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
}
