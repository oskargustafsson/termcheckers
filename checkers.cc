#include <iostream>
#include <cstdlib>
#include "game.h"
#include "ui.h"

using namespace std;
using namespace termcheckers;

int main(int argc, char* argv[]) {
	string line;

	cout << "Hello!\n";

	if(argc > 1) {
		Game* game = Game::instance();
		game->loadGame(argv[1]);
		game->play();
	} else do {
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
		} else if(line == "kingtest") {
			Game* game = Game::instance();
			game->kingGame();
			game->play();
		}
	} while(line != "quit");
	
	cout << "Bye!\n";
}
