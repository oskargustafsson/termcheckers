#ifndef SEARCHTREE_H
#define SEARCHTREE_H

#define DEPTH 10

#include "board.h"
#include "game.h"

using namespace checkers;


int alphabeta(Board& b, int depth, Game* game);
int alphabeta(Board& b, int depth, int alpha, int beta, Game* game);
int captureAlphaBeta(Board& b, int depth, int alpha, int beta, unsigned int from, Game* game);
bool oneCapture(Board b, unsigned int from, unsigned int moves);

#endif
