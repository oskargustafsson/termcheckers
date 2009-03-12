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

	std::string mainmenu[] = {"Play", "New game", "Load game", "Edit board", "Options", "Quit"};
	gui->println("Use j and k to navigate the menu.");
	while(!quit)
	{
		gui->clearScreen();
		gui->printBoard(board);
		gui->printLog();

		switch(gui->menu(mainmenu, 6))
		{
			case 0:
				do
				{
					game = new Game(board, gui);
					if(game->state.action == BLACK_WON)
						gui->println("Black won!");
					else if(game->state.action == WHITE_WON)
						gui->println("White won!");
					else if(game->state.action == QUIT)
					{
						delete game;
						break;
					}
					delete game;
				} while(gui->dialogbox("Play again?"));

				break;
			case 1:
				gui->println("Generating new game.");
				board.new_board();
				break;
			case 2:
				gui->println("Select game file.");
				if(board.load(gui->input().c_str()))
					gui->println("Game loaded.");
				else
					gui->println("Couldnt open file.");
				break;
			case 3:
				gui->edit(board);
				break;
			case 4:
				gui->println("No options available yet.");
				break;
			case 5:
				quit = true;
				gui->quit();
				break;
			default:
				break;
		}
	}

	delete gui;
}
