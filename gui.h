#ifndef GUI_H
#define GUI_H

#include <string>
#include <vector>
#include <stack>
#include "board.h"
#include "game.h"

namespace checkers {

	class Game;

	class GUI {
		public:
			GUI(Game* g);
			~GUI();

			void clearScreen();
			void printBoard(Board& b);
			static void printInt(unsigned int b);
			void printLog();
			void printInfo();
			void println(std::string);
			void gameOver();
			void input();

		private:
			void printWelcome();

			Game* game;
			std::string messages[4];
	};
}

#endif
