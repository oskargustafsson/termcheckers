#include <iostream>
#include <iterator>
#include <string>
#include <cmath>
#include <cstdlib>
#include "board.cc"
#include "checkers.h"
#include "ui.cc"
#include "searchtree.cc"

using namespace std;

int main(int argc, char* argv[]) {
	board b;

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
//	b.black = 0x000000FFF;
//	b.black |= 0x20;
//	b.white = 0x100;
//	b.player = BLACK;
//	b.kings = 0x0;
//	b = createBoard();

//	printBoard(b);
//	DEPTH_TMP = 4;
//	alphabeta(b);

	newGame(b);
	printBoard(b);
}

int newGame(board& b) {
	b = createBoard();
	bool ai;
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
		} else {
			user(b);
		}
	}
}

bool makeMove(board& b, unsigned int from, unsigned int to) {
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

void user(board& b) {
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

	if(!makeMove(b, from, to)) {
		user(b);
	}
}
