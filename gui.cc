#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <cmath>
#include <sstream>
#include "gui.h"
#include "evaluation.h"
#include "game.h"
#include "board.h"

using namespace std;

namespace checkers {

	GUI::GUI(Game* g) : game(g) {
		clearScreen();
		printWelcome();
	}

	GUI::~GUI() {
	}

	void GUI::clearScreen() {
		cout << "\033[2J";
	}

	void GUI::printBoard(Board& board) {
		int row = 0;

		cout << "\033[2;H"; // place cursor at top-left corner

		for(int i=0; i < 32; i++) {
			if(i % 4 == 0) {
				cout << "\033[3G";
				//printf("\033[40m%2c\033[0m", ' ');
			}
			if((i+4) % 8 != 0) {
				printf("\033[47m    \033[0m");
			}
			if(row == 0) {
				if((0x1 & (board.black>>i)) != 0) {
					if((0x1 & (board.kings>>i)) != 0)
						printf("\033[42;1;31m %c%c \033[0m", 'W', 'W');
					else
						printf("\033[42;1;31m %c%c \033[0m", '=', '=');
				} else if((0x1 & (board.white>>i)) != 0) {
					if((0x1 & (board.kings>>i)) != 0)
						printf("\033[42;1;37m %c%c \033[0m", 'W', 'W');
					else
						printf("\033[42;1;37m %c%c \033[0m", '=', '=');
				} else {
					printf("\033[42m    \033[0m");
				}
			} else if(row == 1) {
				printf("\033[42;30m %2d \033[0m", i+1);
			}

			if((i+1) % 4 == 0) {
				if((i+5)% 8 != 0) {
					printf("\033[47m    \033[0m");
				}


				printf("\033[40m%2c\033[0m", ' ');

				printf("\n");
				row++;
				if(row == 2) {
					row = 0;
				} else {
					i -= 4;
				}
			}
		}
	}

	void GUI::printInfo()
	{
		cout << "\033[2;37H"; // place cursor at position 2x37

		cout << "\033[K"; // clear the line
		cout << "\033[32m+-< Game >---------------------+\033[0m";
		cout << "\033[1B"; // move cursor down

		cout << "\033[37G"; // set cursor at column 37
		cout << "\033[K"; // clear the line
		cout << "\033[32m|\033[0m ";
		if(game->board.player == BLACK) {
			cout << "\033[36m";
		}
		cout << "Black (" << game->black->getTime()/1000000 << " sec)\033[0m";
		cout << "\033[32m\033[68G|\033[0m\033[1B";

		cout << "\033[37G"; // set cursor at column 37
		cout << "\033[K"; // clear the line
		cout << "\033[32m|\033[0m ";
		if(game->board.player == WHITE) {
			cout << "\033[36m";
		}
		cout << "White (" << game->white->getTime()/1000000 << " sec)\033[0m";
		cout << "\033[32m\033[68G|\033[0m\033[1B";

		cout << "\033[37G"; // set cursor at column 37
		cout << "\033[K"; // clear the line
		cout << "\033[32m|\033[0m";
		cout << "\033[32m\033[68G|\033[0m\033[1B";

		cout << "\033[37G\033[K";
		cout << "\033[32m|\033[0m Move #: " << game->move_count;
		cout << "\033[32m\033[68G|\033[0m\033[1B";

		cout << "\033[37G"; // set cursor at column 37
		cout << "\033[K"; // clear the line
		cout << "\033[32m|\033[0m";
		cout << "\033[32m\033[68G|\033[0m\033[1B";

		cout << "\033[37G"; // set cursor at column 37
		cout << "\033[K"; // clear the line
		cout << "\033[32m+-< Last move >----------------+\033[0m";
		cout << "\033[1B"; // move cursor down

		cout << "\033[37G"; // set cursor at column 37
		cout << "\033[K"; // clear the line
		cout << "\033[32m|\033[0m Alphabeta value: " << game->lastMove.value;
		cout << "\033[32m\033[68G|\033[0m\033[1B";

		cout << "\033[37G"; // set cursor at column 37
		cout << "\033[K"; // clear the line
		cout << "\033[32m|\033[0m Extended depth: " << game->lastMove.extendedDepth;
		cout << "\033[32m\033[68G|\033[0m\033[1B";

		cout << "\033[37G"; // set cursor at column 37
		cout << "\033[K"; // clear the line
		cout << "\033[32m|\033[0m Calculation depth: " << game->lastMove.depth;
		cout << "\033[32m\033[68G|\033[0m\033[1B";

		cout << "\033[37G"; // set cursor at column 37
		cout << "\033[K"; // clear the line
		cout << "\033[32m|\033[0m Evaluation: " << evaluate(game->board, 0);
		cout << "\033[32m\033[68G|\033[0m\033[1B";

		cout << "\033[37G"; // set cursor at column 37
		cout << "\033[K"; // clear the line
		cout << "\033[32m|\033[0m Nodes visited: " << game->lastMove.nodes;
		cout << "\033[32m\033[68G|\033[0m\033[1B";

		cout << "\033[37G"; // set cursor at column 37
		cout << "\033[K"; // clear the line
		cout << "\033[32m|\033[0m Calculation time: ";
		cout << (game->lastMove.time)/1000000;
		cout << " sec";
		cout << "\033[32m\033[68G|\033[0m\033[1B";

		cout << "\033[37G"; // set cursor at column 37
		cout << "\033[K"; // clear the line
		cout << "\033[32m|\033[0m";
		cout << "\033[32m\033[68G|\033[0m\033[1B";

		cout << "\033[37G"; // set cursor at column 37
		cout << "\033[K"; // clear the line
		cout << "\033[32m|\033[0m";
		cout << "\033[32m\033[68G|\033[0m\033[1B";

		cout << "\033[37G"; // set cursor at column 37
		cout << "\033[K"; // clear the line
		cout << "\033[32m+------------------------------+\033[0m";
	}

	void GUI::printLog()
	{
		cout << "\033[19;2H"; // set cursor at row 18
		for(int i=0; i<4; i++)
		{
			printf("\033[K\033[3G%s\n", messages[i].c_str());
		}
	}

	void GUI::printInt(unsigned int b)
	{
		for(int i=0; i < 32; i++)
		{
			if((i+4) % 8 != 0)
				printf("\033[47m  \033[0m");
			if((0x1 & (b>>i)) != 0)
				printf("\033[40m  \033[0m");
			else
				printf("\033[42m  \033[0m");
			if((i+1) % 4 == 0)
			{
				if((i+5)%8 != 0)
				{
					printf("\033[47m  \033[0m");
				}
				printf("\n");
			}
		}
		printf("\n");
	}

	void GUI::printWelcome()
	{
		cout << "\033[2;1H";
		cout << "__        __   _                            _\n";
		cout << "\\ \\      / /__| | ___ ___  _ __ ___   ___  | |_ ___\n";
		cout << " \\ \\ /\\ / / _ \\ |/ __/ _ \\| '_ ` _ \\ / _ \\ | __/ _ \\\n";
		cout << "  \\ V  V /  __/ | (_| (_) | | | | | |  __/ | || (_) |\n";
		cout << "   \\_/\\_/ \\___|_|\\___\\___/|_| |_| |_|\\___|  \\__\\___/\n";
		cout << "        _                                _\n";
		cout << "       | |    ___ _ __  _ __   __ _ _ __| |_\n";
		cout << "       | |   / _ \\ '_ \\| '_ \\ / _` | '__| __|\n";
		cout << "       | |__|  __/ | | | | | | (_| | |  | |_\n";
		cout << "       |_____\\___|_| |_|_| |_|\\__,_|_|   \\__|\n";
		cout << "\n\n\n";
		cout << " Authors: Johan Ask, Linus Probert and Oskar Gustafsson.\n";
	}

	void GUI::println(string str)
	{
		for(int i=0; i<3; i++)
		{
			messages[i] = messages[i+1];
		}
		messages[3] = str;
		printLog();
	}

	void GUI::gameOver() {
		cout << "\033[8;H\033[37;41m";
		cout << "\033[8G +------------------+ \n";
		cout << "\033[8G | GAME OVER BITCH! | \n";
		cout << "\033[8G |    You Lose!     | \n";
		cout << "\033[8G +------------------+ \n";
		cout << "\033[0m";
	}

	void GUI::input()
	{
		string command;
		cout << "\033[23;1H>\033[K ";
		cout << "\033[?25h";
		cin >> command;

		game->interpretCommand(command);
	}
}
