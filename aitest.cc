#include <iostream>
#include <sstream>
#include <vector>
#include "gui.h"
#include "board.h"
#include "game.h"
#include "functions.h"

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
		Search s(game);
		s.search();
		nodes += s.nrOfNodes;
		nodes /= 2;
		depth += s.maxdepth;
		depth /= 2;
		i++;
	}
	cout << "DONE" << std::endl;
	cout << "avg depth: " << depth << " avg nodes: " << nodes << "\n";

//	Search s(game);
//	s.sortTest();

	delete game;
	delete gui;
}
