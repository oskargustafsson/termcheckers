#include <iostream>
#include <sstream>
#include "gui.h"
#include "board.h"
#include "game.h"

#define AI_TEST

using namespace checkers;
using namespace std;

int main() {

	Game* game = new Game();
	GUI* gui = new GUI(game);
	game->setGUI(gui);

	Board& board = game->board;
	board.createBoard();

	int i=0;
	while(!game->board.endOfGame() && i < 100) {
		gui->printBoard(game->board);
		game->ai();
		i++;
	}
	std::cout << "DONE" << std::endl;


	delete game;
	delete gui;
}
