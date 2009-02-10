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

int main() {
	board b = createBoard();
	string line;

	while(!endOfGame(b)) {

		printBoard(b);

		if(b.player == WHITE) {
			cout << "White: ";
		}else if(b.player == BLACK) {
			cout << "Black: ";
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
			changePlayer(b);
		}
	}
	printBoard(b);
}
