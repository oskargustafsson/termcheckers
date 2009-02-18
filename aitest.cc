#include <iostream>
#include "gui.h"
#include "board.h"
#include "game.h"

#define AI_TEST

using namespace checkers;

int main() {

	Game* game = new Game();
	GUI* gui = new GUI(game);
	game->setGUI(gui);

	Board& board = game->board;
	board.createBoard();

	int i=0;
	while(!game->board.endOfGame() && i < 300) {
		Search search(game);
		search.search();
		gui->printBoard(game->board);
		i++;
	}
	std::cout << "DONE" << std::endl;


	delete game;
	delete gui;
}
