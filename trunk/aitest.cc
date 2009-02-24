#include <iostream>
#include <sstream>
#include <vector>
#include "gui.h"
#include "board.h"
#include "game.h"
#include "functions.h"
#include "search.h"

#define AI_TEST

using namespace checkers;
using namespace std;

int main() {

	float depth = 0;
	float nodes = 0;

	Game* game = new Game();
	GUI* gui = new GUI(game);
	game->setGUI(gui);

	Board& board = game->board;
	board.createBoard();

	int i=0;
	while(!game->board.endOfGame() && i < 100) {
		Search s;
		SearchResult result = s.search(game->board, 1000000);
		gui->printBoard(game->board);
		nodes += result.nodes;
		nodes /= 2;
		depth += result.depth;
		depth /= 2;
		i++;
	}
	cout << "DONE" << std::endl;
	cout << "avg depth: " << depth << " avg nodes: " << nodes << "\n";

	delete game;
	delete gui;
}
