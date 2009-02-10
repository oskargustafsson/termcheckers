#include <iostream>
#include <iterator>
#include <string>
#include <cmath>
#include <cstdlib>
#include "board.cc"
#include "ui.cc"

using namespace std;

int main() {
	board b = createBoard();
	string line;

	displayBoard(b);
	while(!endOfGame(b)) {

		cout << endl;

		if(b.player == WHITE) {
			cout << "White: ";
		}else if(b.player == BLACK) {
			cout << "Black: ";
		}

		cin >> line;
		if(line == "quit") {
			break;
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
			from = pow(2, atoi(first.c_str())-1);
			to = pow(2, atoi(second.c_str())-1);

			cout << from << endl;
			cout << to << endl;

			if(!move(b, from, to)) {
				cout << "Illegal move.\n";
				continue;
			}
		}

		displayBoard(b);

		if(b.player == WHITE) {
			b.player = BLACK;
		} else {
			b.player = WHITE;
		}
	}
}
