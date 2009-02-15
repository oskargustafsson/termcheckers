#ifndef SEARCHTREE_H
#define SEARCHTREE_H

#define DEPTH 10

#include "board.h"

int alphabeta(board& b, int depth);
int alphabeta(board& b, int depth, int alpha, int beta);
int captureAlphaBeta(board& b, int depth, int alpha, int beta, unsigned int from);
bool oneCapture(board b, unsigned int from, unsigned int moves);

#endif
