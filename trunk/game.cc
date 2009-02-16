#include <iostream>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <stack>
#include <vector>
#include "game.h"
#include "search.h"
#include "board.h"
#include "gui.h"

using namespace std;

namespace checkers {

  Game::Game() {
    state = NOT_PLAYING;
    //board.compute_bits_in_char();
  }
  Game::~Game() {}


  void Game::setGUI(GUI* g) {
    gui = g;
  }

	bool Game::makeMove(vector<unsigned int> movements) {
		int size = movements.size();
		if(!board.validateMove(movements[0], movements[size-1])) {
			return false;
		} else {
			history.push(board);
			if((board.getCaptureMoves(movements[0]) == 0) && size == 2) {
				board.move(movements[0], movements[1]);
			} else {
				for(int i=1; i<size; i++) {
					if(recursiveCapture(board, movements[i-1], movements[i])) {
					} else {
						cout << "Illegal move: " << log2(movements[i-1])+1 << "-" << log2(movements[i])+1 << endl;
						undoLastMove();
						return false;
					}
				}
			}
			cout << "My move is: " << log2(movements[0])+1;
			for(int i=1; i<size; i++) {
				cout << "-" << log2(movements[i])+1;
			}
			cout << endl;
			board.updateKings();
			board.changePlayer();
			return true;
		}
	}

  bool Game::recursiveCapture(Board tmpboard, unsigned int from, unsigned int to) {
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
        return true;
      }
      if(recursiveCapture(test, capture, to)) {
        return true;
      }
    }
    return false;
  }

  void Game::newGame() {
    board.createBoard();
  }

  void Game::loadGame(char* file) {
    ifstream is;
    is.open(file);
    unsigned int piece = 0x1;

    if(is != NULL) {
      cout << "Loading file...\n";
      char ch;
      while(is.get(ch) != NULL) {
        cout << ch;
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
    }
  }

	void Game::aiTest() {
		while(!board.endOfGame()) {
			Search search(this);
			gui->printBoard(board);
			search.search();
		}
	}

	void Game::ai() {
			Search search(this);
			search.search();
			gui->printBoard(board);
	}

  void Game::play() {
    state = PLAYING;
    gui->printBoard(board);
    while(!board.endOfGame()) {
      gui->input();
    }
    cout << "Game over...\n";
  }

  void Game::stop() {
  }

  bool Game::undoLastMove() {
    if(history.empty()) {
      return false;
    } else {
      board = history.top();
      history.pop();
      return true;
    }
  }
}
