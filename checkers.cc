#include <iostream>
#include <cstdlib>
#include "game.h"
#include "ui.h"

using namespace std;
using namespace termcheckers;

int main(int argc, char* argv[]) {
	string line;

	cout << "Hello!\n";

	Game* game = Game::instance();
	game->newGame();

	if(argc > 1) {
		game->loadGame(argv[1]);
	}
	do {
		cout << "#> ";
		cin >> line;
		
		if(line == "play") {
			game->play();
		} else if(line == "printboard") {
			printBoard(createBoard());
		} else if(line == "aitest") {
			game->aiTest();
		} else if(line == "newgame") {
			game->newGame();
			game->play();
		}
	} while(line != "quit");
	
	cout << "Bye!\n";
}
