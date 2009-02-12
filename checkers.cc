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
			Game* game = Game::instance();
			game->newGame();
			game->play();

		} else if(line == "tabletest") {
			cout << "IMPLEMENT\n";

		} else if(line == "printboard") {
			printBoard(createBoard());
		}
	} while(line != "quit");
	
	cout << "Bye!\n";
}
