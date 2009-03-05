#include "game.h"
#include "gui.h"

using namespace checkers;

int main(int argc, char* argv[]) {

	Game* game = new Game();
	GUI* gui = new GUI(game);
	game->setGUI(gui);

	if(argc > 1) {
		game->load(argv[1]);
	}
	else
	{
		game->board.createBoard();
	}

	while(game->state != QUIT) {
		gui->input();
	}
	delete game;
	delete gui;
}
