#include <iostream>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <stack>
#include "game.h"
#include "searchtree.h"
#include "board.h"
#include "ui.h"

using namespace std;

namespace termcheckers {

        Game::Game() {
                compute_bits_in_char();
        }
        Game::~Game() {
                delete game;
        }

        bool Game::makeMove(unsigned int from, unsigned int to) {
                if(!validateMove(b, from, to)) {
                        return false;
                } else {
                  stackBoard();
                        if(getCaptureMoves(b, from) == 0) {
                                move(b, from, to);
                        } else {
                                if(recursiveCapture(b, from, to)) {
				} else {
					cout << "Erroronous move: " << log2(from)+1 << "-" << log2(to)+1 << endl;
					return false;
				}
                        }

                        changePlayer(b);
                        cout << "My move is: " << log2(from)+1 << "-" << log2(to)+1 << endl;
                        return true;
                }
        }

        bool Game::recursiveCapture(board tmpboard, unsigned int from, unsigned int to) {
                unsigned int moves = getCaptureMoves(tmpboard, from);
                unsigned int capture = 0x0;
                board test;
                while(moves != 0) {
                        capture = (moves & (moves-1)) ^ moves;
                        moves &= moves-1;
                        test = tmpboard;
                        move(test, from, capture);

                        if(capture == to) {
                                b = test;
                                return true;
                        }
                        if(recursiveCapture(test, capture, to)) {
                                return true;
                        }
                }
                return false;
        }

        void Game::newGame() {
                b = createBoard();
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
						b.player = BLACK;
					}
					b.black |= piece;
					piece = piece<<1;
				} else if(ch == 'B') {
					b.black |= piece;
					b.kings |= piece;
					piece = piece<<1;
				} else if(ch == 'w') {
					if(piece == 0) {
						b.player = WHITE;
					}
					b.white |= piece;
					piece = piece<<1;
				} else if(ch == 'W') {
					b.white |= piece;
					b.kings |= piece;
					piece = piece<<1;
				} else if(ch == '.') {
					piece = piece<<1;
				}
			}
			is.close();
		}
	}

	void Game::kingGame() {
		b.white = 0x8000000;
		b.black = 0x24000;
		b.kings = b.black|b.white;
		b.player = BLACK;
	}

		void Game::aiTest() {
			while(!endOfGame(b)) {
				printBoard(b);
				alphabeta(b, DEPTH);
			}
		}

        void Game::play() {
			string line;
			unsigned int from = 0x0;
	        unsigned int to = 0x0;

			printBoard(b);
            while(!endOfGame(b)) {

			cout << "#> ";
			cin >> line;

            string first;
            string second;
            string::iterator It = line.begin();
	        int i=0;

       		while( It != line.end()) {
               	if( *It == '-') {
                	i++;
                    It++;
                    continue;
	            }
                if(i == 0)
                	first += *It;
                    if(i == 1)
                    	second += *It;
						It++;
                }
                from = pow(2.0, atof(first.c_str())-1);
	            to = pow(2.0, atof(second.c_str())-1);

				if(from != 0 && to != 0)
				{
					if(!makeMove(from, to))
						cout << "Illigal move!\n";
					else
						printBoard(b);
				}
				else if(line == "ai")
				{
					alphabeta(b, DEPTH);
					printBoard(b);
				}
				else if(line == "undo" || line == "back")
				{
					if(undoLastMove()) {
						cout << "Reverting!" << endl;
						printBoard(b);
					} else {
						cout << "Nothing to undo!" << endl;
					}
				}
				else if(line == "help")
				{
					cout << "Commands: ai, undo, print, quit\n";
					cout << "Move: from-to\n";
				}
				else if(line == "print" || line == "board")
				{
					printBoard(b);
				}
				else if(line == "quit" || line == "exit")
				{
					cout << "Exit current game" << endl;
					break;
				}
            }
        }

        Game* Game::game = NULL;

        Game* Game::instance() {
                if(game == NULL) {
                        game = new Game();
                }
                return game;
        }

  bool Game::undoLastMove() {
    if(history.empty()) {
      return false;
    } else {
      b = history.top();
      history.pop();
      return true;
    }
  }

  void Game::stackBoard() {
    history.push(b);
  }
}
