#ifndef GAME_H
#define GAME_H

#include "board.h"
#include <stack>

namespace termcheckers {

        class Game {
        public:
                ~Game();
                bool makeMove(unsigned int from, unsigned int to);
                void newGame(int black, int white, bool output);
                void play();
                static Game* instance();
                int depth;
        private:
                Game();
                void user();
                bool recursiveCapture(board tmpboard, unsigned int from, unsigned int to);

                bool undoLastMove();
                void stackBoard();

                static Game* game;
                std::stack<board> history;
                board b;
                int black_player;
                int white_player;
                bool output;
        };
}
#endif
