#include <iostream>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <stack>
#include <vector>
#include <sstream>
#include "game.h"
#include "search.h"
#include "board.h"
#include "gui.h"
#include "functions.h"
#include "transposition.h"

using namespace std;

namespace checkers {

  Game::Game() {
    state = NOT_PLAYING;
    board_count = 0;		// put elsewhere?
    blackTime = 0;
    whiteTime = 0;
  }
  Game::~Game()
  {
  }

  void Game::setGUI(GUI* g) {
    gui = g;
  }

  bool Game::makeMove(vector<unsigned int>& movements) {
    ostringstream oss; //for gui output
    ostringstream message;
    int size = movements.size();

	 message << "My move is " << log2(movements[0])+1;
	 for(int i = 1; i<size; i++) {
		message << "-" << log2(movements[i])+1;
	 }
	 gui->println(message.str());
	 message.flush();

    int result = board.validateMove(movements);
    /////////////////////
    // result:
    // 0 Legal move.
    // -1 illegal
    // -2 more captures possible
    // -3 movements.size() < 2
    //////////////////
    if(result == 0) {
	    if(!history.empty()) updateBoardHistory(board, history.top());
	    history.push(board);

	    if(size == 2 && board.getCaptureMoves(movements[0]) == 0) {
		    board.move(movements[0], movements[1]);
	    }
	    else {
		    for(int i=1; i<size; i++)
		    {
			    recursiveCapture(board, movements[i-1], movements[i]);
		    }
	    }
	    board.updateKings();
	    board.changePlayer();

	    // GUI:
	    gui->editMoveCounter(1);
	    gui->stackLastMove();
	    oss << "Last move: " << log2(movements[0])+1;
	    for(int i = 1; i<size; i++) {
		    oss << "-" << log2(movements[i])+1;
	    }
    }
    else if(result == -1) {
	    oss << "\033[31mIllegal move!\033[0m";
    }
    else if(result == -2) {
	    oss << "\033[31mMore captures possible!\033[0m";
    }
    else if(result == -3) {
	    oss << "\033[31mNot enough moves\033[0m";
    }

    gui->setInfo(oss.str(), "LM"); //Send info to gui.
    oss.flush();

    if(result == 0) {
         }
    return result == 0;
  }
  
	void Game::updateBoardHistory(Board& newboard, Board& lastboard) {
	  // the only time we need to add a board, is when a king moves
		if( countBits(lastboard.kings ^ newboard.kings) == 2 )
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

  int Game::recursiveCapture(Board tmpboard, unsigned int from, unsigned int to) {
    unsigned int moves = tmpboard.getCaptureMoves(from);
    unsigned int capture = 0x0u;
    Board test;
    while(moves != 0) {
      capture = (moves & (moves-1)) ^ moves;
      moves &= moves-1;
      test = tmpboard;

      test.move(from, capture);

      if(capture == to) {
        board = test;
        return 0;
      }
      if(recursiveCapture(test, capture, to) == 0) {
        return 0;
      }
    }
    return -1;
  }

  void Game::newGame() {
    board.createBoard();
  }

  void Game::loadGame(char* file) {
    ifstream is;
    is.open(file);
    unsigned int piece = 0x1;

    gui->println("Loading file...");
    if(is != NULL) {
      char ch;
      while(is.get(ch) != NULL) {
        if(ch == 'b') {
          if(piece == 0) {
            board.player = BLACK;
          }
          board.black |= piece;
          board.white &= ~piece;
          board.kings &= ~piece;
          piece = piece<<1;
        } else if(ch == 'B') {
          board.black |= piece;
          board.kings |= piece;
          board.white &= ~piece;
          piece = piece<<1;
        } else if(ch == 'w') {
          if(piece == 0) {
            board.player = WHITE;
          }
          board.white |= piece;
          board.black &= ~piece;
          board.kings &= ~piece;
          piece = piece<<1;
        } else if(ch == 'W') {
          board.white |= piece;
          board.kings |= piece;
          board.black &= ~piece;
          piece = piece<<1;
        } else if(ch == '.') {
          board.black &= ~piece;
          board.white &= ~piece;
          board.kings &= ~piece;
          piece = piece<<1;
        }
      }
      is.close();
      gui->println("DONE!");
    }
    else
    {
      gui->println("Couldn't open file.");
    }
  }

        void Game::ai() {
                        Search search;
			SearchResult result;
			int time;
			int time_used;

			if(board.player == BLACK)
				time_used = blackTime;
			else
				time_used = whiteTime;
			if(TOTAL_TIME - time_used > 100000000)
				time = 10000000;
			else if(TOTAL_TIME - time_used > 50000000)
				time = 9000000;
			else if(TOTAL_TIME - time_used > 20000000)
				time = 8000000;
			else if(TOTAL_TIME - time_used > 10000000)
				time = 6000000;
			else if(TOTAL_TIME - time_used > 5000000)
				time = 3000000;
			else if(TOTAL_TIME - time_used > 1000000)
				time = 1000000;
			else
				time = 10000;

                        result = search.search(board, time);
			if(board.player == BLACK)
				blackTime += result.time;
			else
				whiteTime += result.time;
			makeMove(result.move);
                        ostringstream value;
			value << result.value;
                        ostringstream depth;
                        depth << result.depth;
                        ostringstream nodes;
                        nodes << result.nodes;
                        ostringstream time_str;
                        time_str << result.time;
                        ostringstream extdepth;
                        extdepth << result.extendedDepth;

                        gui->setInfo(value.str(), "VALUE");
                        gui->setInfo(time_str.str(), "TIME");
                        gui->setInfo(nodes.str(), "NODES");
                        gui->setInfo(depth.str(), "DEPTH");
                        gui->setInfo(extdepth.str(), "EXTDEPTH");
                        value.flush();
                        depth.flush();
                        nodes.flush();
                        time_str.flush();
                        extdepth.flush();
        }

  void Game::play() {
    state = PLAYING;
    gui->printBoard(board);
    while(!board.endOfGame() && state == PLAYING) {
      gui->input();
    }
    state = NOT_PLAYING;
    gui->gameOver();
  }

  void Game::stop() {
  }

  bool Game::undoLastMove() {
    if(history.empty()) {
      return false;
    } else {
      gui->editMoveCounter(-1);
      gui->popLastMove();
      board = history.top();
      history.pop();
      return true;
    }
  }
}
