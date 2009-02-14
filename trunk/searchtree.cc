#include <cstdio>
#include "searchtree.h"
#include "evaluation.h"
#include "board.h"
#include "game.h"
#include "ui.h"

using namespace termcheckers;

int maxdepth;

int alphabeta(board& b, int depth) {
	maxdepth = depth;
	return alphabeta(b, depth, -32000, 32000);
}

unsigned int recursiveTo = 0x0;

int alphabeta(board& b, int depth, int alpha, int beta) {
	unsigned int men;
	unsigned int moves;
	int tmp;
	bool betacutoff = false;
	unsigned int moveFrom = 0x0;
	unsigned int from = 0x0;
	unsigned int to = 0x0;
	unsigned int moveTo = 0x0;
	int capture = 0;

	b.player == WHITE ? men = b.white : men = b.black;

	while(men != 0) {
		from = (men & (men-1)) ^ men;
		men &= men-1;
		if(getCaptureMoves(b, from) != 0) {
			capture++;
		}
	}
	
	// if there is capture moves try one depth more
	if(endOfGame(b) || ((depth < 1) && (capture == 0))) {
		if(b.player == BLACK) {
			return evaluate(b);
		} else {
			return -evaluate(b);
		}
	}

	b.player == WHITE ? men = b.white : men = b.black;

	while(men != 0) {
		from = (men & (men-1)) ^ men;
		men &= men-1;

/*
 * FIX THIS: break if there is only one capturemove to do
 * 			if((capture == 1) && (depth == maxdepth)) {
			moves = getCaptureMoves(b, from);
			if(moves != 0) {
				moveFrom = from;
				moveTo = moves;
				break;
			}
		} else*/ if(capture == 0) {
			moves = getMoves(b, from);
		} else {
			moves = getCaptureMoves(b, from);
		}

		while(moves != 0) {
			to = (moves & (moves-1)) ^ moves;
			moves &= moves-1;

			board nextboard = b;
			move(nextboard, from, to);

			if(capture != 0) {
				tmp = captureAlphaBeta(nextboard, depth, alpha, beta, to);
				to = recursiveTo;
			} else {
				changePlayer(nextboard);
				tmp = -alphabeta(nextboard, depth-1, -beta, -alpha);
			}
			if(tmp > alpha) {
				alpha = tmp;
				moveFrom = from;
				moveTo = to;
			}
			if(beta <= alpha) {
				betacutoff = true;
				break;
			}
		}
		if(betacutoff) {
			break;
		}
	}
	if(depth == maxdepth) {
		unsigned int places[] = {moveFrom, moveTo};
		// The root node, make the best move
		Game::instance()->makeMove(places, 2);
	}
	return alpha;
}

int captureAlphaBeta(board& b, int depth, int alpha, int beta, unsigned int from) {
	unsigned int moves = getCaptureMoves(b, from);
	unsigned int to = 0x0;
	int tmp;

	if(moves == 0) {
		changePlayer(b);

		tmp = -alphabeta(b, depth-1, -beta, -alpha);
		if(tmp > alpha) {
			alpha = tmp;
			if(depth == maxdepth) {
				recursiveTo = from;
			}
		}
	}

	while(moves != 0) {
		to = (moves & (moves-1)) ^ moves;
		moves &= moves-1;

		board nextboard = b;
		move(nextboard, from, to);

		tmp = captureAlphaBeta(nextboard, depth, alpha, beta, to);
		if(tmp > alpha) {
			alpha = tmp;
		}
	}

	return alpha;
}
