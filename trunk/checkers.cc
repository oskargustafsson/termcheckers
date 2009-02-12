#include <iostream>
#include <cstdlib>
#include "checkers.h"
#include "game.h"
#include "ui.h"

using namespace std;
using namespace termcheckers;

int main(int argc, char* argv[]) {

	for(int i=1; i<argc; i++) {
		string arg = argv[i];
		if(arg == "-q") {
		}
		if(arg == "-w") {
		}
		if(arg == "-b") {
		}
	}
	Game* game = Game::instance();
	game->newGame(3, 3);
	game->play();

}


