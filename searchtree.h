#ifndef SEARCHTREE_H
#define SEARCHTREE_H

#define DEPTH 9

#include "board.h"

int alphabeta(board& b, int depth);
int alphabeta(board& b, int depth, int alpha, int beta);
int captureAlphaBeta(board& b, int depth, int alpha, int beta, unsigned int from);

#endif
