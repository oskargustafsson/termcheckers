#ifndef GAME_H
#define GAME_H

#include "board.h"
#include <stack>
#include <vector>
#include "search.h"

#define NOT_PLAYING 0
#define PLAYING 1
#define QUIT 2

namespace checkers {
		class GUI;

        class Game {
        public:
                Game();
                ~Game();
					 void setGUI(GUI* g);
                bool makeMove(std::vector<int> movements, int size);
                void newGame();
                void loadGame(char* file);
                void play();
					 void stop();
                void aiTest();
                void ai();
                bool undoLastMove();

                Board board;
					 int state;
        private:
                bool recursiveCapture(Board tmpboard, unsigned int from, unsigned int to);

                std::stack<Board> history;
                bool output;
					 GUI* gui;
        };
}
#endif