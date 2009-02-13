#ifndef GAME_H
#define GAME_H

#include "board.h"
#include <stack>

namespace termcheckers {

        class Game {
        public:
                ~Game();
                bool makeMove(unsigned int from, unsigned int to);
                void newGame();
                void kingGame();
                void loadGame(char* file);
                void play();
                void aiTest();
                static Game* instance();
        private:
                Game();
                bool recursiveCapture(board tmpboard, unsigned int from, unsigned int to);

                bool undoLastMove();
                void stackBoard();

                static Game* game;
                std::stack<board> history;
                board b;
                bool output;
        };
}
#endif
