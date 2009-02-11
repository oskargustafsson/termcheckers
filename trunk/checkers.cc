#include <iostream>
#include <iterator>
#include <string>
#include <cmath>
#include <cstdlib>
#include "checkers.h"
#include "board.cc"
#include "ui.cc"
#include "searchtree.cc"

using namespace std;

int main(int argc, char* argv[]) {
	board b = createBoard();
	string line;
	bool ai;

	OUTPUT = true;
	WHITE_AI = false;
	BLACK_AI = false;
	WHITE_DEPTH = 1;
	BLACK_DEPTH = 1;

	for(int i=1; i<argc; i++) {
		string arg = argv[i];
		if(arg == "-q") {
			OUTPUT = false;
		}
		if(arg == "-w") {
			i++;
			WHITE_AI = true;
			WHITE_DEPTH = atoi(argv[i]);
		}
		if(arg == "-b") {
			i++;
			BLACK_AI = true;
			BLACK_DEPTH = atoi(argv[i]);
		}
	}

	while(!endOfGame(b)) {

		if(OUTPUT) {
			printBoard(b);
		}

		currentplayer = b.player;

		if(b.player == WHITE) {
			DEPTH_TMP = WHITE_DEPTH;
			ai = WHITE_AI;
			cout << "White: ";
		}else if(b.player == BLACK) {
			DEPTH_TMP = BLACK_DEPTH;
			ai = BLACK_AI;
			cout << "Black: ";
		}

		if(ai) {
			alphabeta(b);
			continue;
		}

		cin >> line;
		if(line == "quit") {
			break;
		} else if(line == "ai") {
			alphabeta(b);
		} else {
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

			cout << "From: " << from << endl;
			cout << "To: " << to << endl;

			if(!move(b, from, to)) {
				cout << "Illegal move.\n";
				continue;
			}
		}
	}
	printBoard(b);
}
