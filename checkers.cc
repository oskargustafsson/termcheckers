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
		
		if(line == "play" || line == "newgame") {
			Game* game = Game::instance();
			game->newGame();
			game->play();

		} else if(line == "printboard") {
			printBoard(createBoard());
		} else if(line == "aitest") {
			Game* game = Game::instance();
			game->newGame();
			game->aiTest();
		}
	} while(line != "quit");
	
	cout << "Bye!\n";
}
