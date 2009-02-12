#include <iostream>
#include <cmath>
#include <cstdlib>
#include "game.h"
#include "searchtree.h"
#include "board.h"
#include "ui.h"

using namespace std;

namespace termcheckers {

	Game::Game() {
		black_player = 0;
		white_player = 0;
		output = true;
	}
	Game::~Game() {
		delete game;
	}

	bool Game::makeMove(unsigned int from, unsigned int to) {
		if(false) {
			cout << "Illegal move.\n";
			return false;
		} else {
			move(b, from, to);
			changePlayer(b);
			cout << log2(from)+1 << "-" << log2(to)+1 << endl;
			return true;
		}
	}

	void Game::user() {
		string line;
		cin >> line;

		unsigned int from = 0x0;
		unsigned int to = 0x0;
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

		if(!makeMove(from, to)) {
			user();
		}
	}

	void Game::newGame(int black, int white, bool output) {
		black_player = black;
		white_player = white;
		output = output;
		b = createBoard();

	}

	void Game::play() {

		while(!endOfGame(b)) {

			if(b.player == WHITE) {
				depth = white_player;
			}else if(b.player == BLACK) {
				depth = black_player;
			}

			if(output) {
				printBoard(b);
				cout << ">> ";
			}

			if(depth > 0) {
				alphabeta(b, depth);
			} else {
				user();
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
}
