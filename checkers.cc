#include <iostream>
#include <cstdlib>
#include "game.h"
#include "ui.h"

using namespace std;
using namespace termcheckers;

int main() {
	string line;

	cout << "Hello!\n";

	do {
		cout << "#> ";
		cin >> line;
		
		if(line == "newgame") {
			int tmp;
			int black = -1;
			int white = -1;

			while(black == -1) {
				cout << "Black: ";
				cin >> line;
				if(line == "human")
					black = 0;
				if(line == "ai") {
					do {
						cout << "Depth: ";
						cin >> line;
						tmp = atoi(line.c_str());
						if(tmp != 0) {
							black = tmp;
						}
					} while(tmp == 0);
				}
			}
			while(white == -1) {
				cout << "White: ";
				cin >> line;
				if(line == "human")
					white = 0;
				if(line == "ai") {
					do {
						cout << "Depth: ";
						cin >> line;
						tmp = atoi(line.c_str());
						if(tmp != 0) {
							white = tmp;
						}
					} while(tmp == 0);
				}
			}

			Game* game = Game::instance();
			game->newGame(black, white, true);
			game->play();

		} else if(line == "tabletest") {
			cout << "IMPLEMENT\n";

		} else if(line == "printboard") {
			printBoard(createBoard());
		}
	} while(line != "quit");
	
	cout << "Bye!\n";
}
