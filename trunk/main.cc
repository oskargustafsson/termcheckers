#include "game.h"
#include "gui.h"

using namespace checkers;

int main(int argc, char* argv[]) {

	GUI* gui = new GUI();
	Board board;
	Game* game;
	bool quit = false;

	if(argc > 1)
	{
		gui->println("Loading file...");
		if(board.load(argv[1]))
			gui->println("DONE!");
		else
			gui->println("Failed");
	}

	std::string mainmenu[] = {"Play", "New game", "Load game", "Options", "Quit"};
	gui->println("Use j and k to navigate the menu.");
	while(!quit)
	{
		gui->clearScreen();
		gui->printBoard(board);
		gui->printLog();

		switch(gui->menu(mainmenu, 5))
		{
			case 0:
				game = new Game(board, gui);
				board = game->board;
				delete game;
				break;
			case 1:
				gui->println("Generating new game");
				board.new_board();
				break;
			case 4:
				quit = true;
				gui->quit();
				break;
			default:
				break;
		}
	}

	delete gui;
}
