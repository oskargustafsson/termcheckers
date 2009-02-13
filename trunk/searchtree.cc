#include <cstdio>
#include "searchtree.h"
#include "evaluation.h"
#include "board.h"
#include "game.h"

using namespace termcheckers;

int alphabeta(board& b, int depth) {
	return alphabeta(b, depth, -32000, 32000);
}

unsigned int recursiveTo = 0x0;

int alphabeta(board& b, int depth, int alpha, int beta) {
	int men;
	int moves;
	int tmp;
	bool betacutoff = false;
	int moveFrom = 0x0;
	int moveTo = 0x0;
	int capture = 0;

	if(b.player == WHITE) {
		men = b.white;
	} else {
		men = b.black;
	}

	/**
	 * TODO:
	 * OPTIMIZE THIS!!
	 */
	for(int i=0x1; i != 0; i = (i<<1)) {
		if((i & men) == 0)
			continue;
		if(getCaptureMoves(b, i) != 0) {
			capture++;
		}
	}
	
	if(endOfGame(b) || ((depth < 1) && (capture == 0))) {
		if(b.player == BLACK) {
			return evaluate(b);
		} else {
			return -evaluate(b);
		}
	}

	while(men != 0) {
		int from = (men & (men-1)) ^ men;
		men &= men-1;

/*		if((capture == 1) && (depth == DEPTH)) {
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
			unsigned int to = (moves & (moves-1)) ^ moves;
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
	if(depth == DEPTH) {
		// The root node, make the best move
		Game::instance()->makeMove(moveFrom, moveTo);
	}
	return alpha;
}

int captureAlphaBeta(board& b, int depth, int alpha, int beta, unsigned int from) {
	unsigned int moves = getCaptureMoves(b, from);
	int tmp;

	if(moves == 0) {
		changePlayer(b);
		tmp = -alphabeta(b, depth-1, -beta, -alpha);
		if(tmp > alpha) {
			alpha = tmp;
			recursiveTo = from;
		}
	}

	while(moves != 0) {
		unsigned int to = (moves & (moves-1)) ^ moves;
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
