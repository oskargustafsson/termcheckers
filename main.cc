#include <iostream>
#include <cstdlib>
#include "game.h"
#include "gui.h"

using namespace std;
using namespace checkers;

int main(int argc, char* argv[]) {
	string line;

	Game* game = new Game();
	GUI* gui = new GUI(game);
	game->setGUI(gui);

	if(argc > 1) {
		game->loadGame(argv[1]);
	}

	while(game->state != QUIT) {
		gui->input();
	}
	delete game;
	delete gui;
}
