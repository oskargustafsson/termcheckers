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

	Game* game = new Game();
	GUI* gui = new GUI(game);
	game->setGUI(gui);

	Board& board = game->board;
	board.createBoard();

	int i=0;
	while(!game->board.endOfGame() && i < 1000) {
		Search s(*game);
		SearchResult result = s.search(game->board, 10000000);
		game->makeMove(result.move);

		ostringstream value;
		value << result.value;
		ostringstream depth;
		depth << result.depth;
		ostringstream nodes;
		nodes << result.nodes;
		ostringstream time_str;
		time_str << result.time;
		ostringstream extdepth;
		extdepth << result.extendedDepth;

		gui->setInfo(value.str(), "VALUE");
		gui->setInfo(time_str.str(), "TIME");
		gui->setInfo(nodes.str(), "NODES");
		gui->setInfo(depth.str(), "DEPTH");
		gui->setInfo(extdepth.str(), "EXTDEPTH");
		value.flush();
		depth.flush();
		nodes.flush();
		time_str.flush();
		extdepth.flush();gui->printBoard(game->board);
		i++;
	}

	delete game;
	delete gui;
}
