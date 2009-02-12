#include <iostream>
#include <cstdlib>
#include "checkers.h"
#include "game.h"
#include "ui.h"

using namespace std;
using namespace termcheckers;

int main() {

	Game* game = Game::instance();
	game->newGame();
	game->play();

}


