#ifndef GUI_H
#define GUI_H

#include <string>
#include <vector>
#include <stack>
#include <termios.h>
#include "board.h"
#include "game.h"

namespace checkers {

	class Game;

	class GUI {
		public:
			GUI();
			~GUI();

			void clearScreen();
			void printBoard(const Board&);
			static void printInt(unsigned int b);
			void printLog();
			void printInfo(Game&);
			void println(std::string);
			int menu(std::string[], int);
			bool dialogbox(std::string);
			void messagebox(std::string);
			void quit();
			void gameOver();
			std::string input();

		private:
			void printWelcome();
			unsigned char getch();

			Game* game;
			std::string messages[4];
	};

	// some stuff for the getch function
	static struct termios termattr, save_termattr;
	static int ttysavefd = -1;
	static enum 
	{ 
		RESET, RAW, CBREAK 
	} ttystate = RESET;

}

#endif
