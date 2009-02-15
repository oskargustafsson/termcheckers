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

unsigned int recursiveTo = 0x0u;

int alphabeta(board& b, int depth, int alpha, int beta) {
	unsigned int men;
	unsigned int moves;
	int tmp;
	bool betacutoff = false;
	unsigned int moveFrom = 0x0u;
	unsigned int from = 0x0u;
	unsigned int to = 0x0u;
	unsigned int moveTo = 0x0u;
	int capture = 0;

	b.player == WHITE ? men = b.white : men = b.black;

	/*
	 * test optimiering:
	 * ta bort några som garanterat inte kan hoppa
	 * inte säkert att det är bättre. och i vilket fall som helst så blir det nog ingen större skillnad...
	*/
	men = (((men<<9) & ~(b.black & b.white))>>9) |
		(((men<<7) & ~(b.black & b.white))>>7) |
		(((men>>9) & ~(b.black & b.white))<<9) |
		(((men>>7) & ~(b.black & b.white))<<7);

	// Count number of capture moves:
	// TODO: try and do this without loop!
	while(men != 0) {
		from = (men & (men-1)) ^ men;
		men &= men-1;
		if(getCaptureMoves(b, from) != 0) {
			capture++;
		}
	}
	
	// Check if its the end node
	// if there is capture moves try one depth more
	if(endOfGame(b) || ((depth < 1) && (capture == 0))) {
		return b.player == BLACK ? evaluate(b) : -evaluate(b);
	}

	// For each move
	b.player == WHITE ? men = b.white : men = b.black;
	while(men != 0) {
		from = (men & (men-1)) ^ men;
		men &= men-1;

		if(capture == 0) {
			moves = getMoves(b, from);
		} else {
			moves = getCaptureMoves(b, from);

 			// break if there is only one capture move to do
			// maybe move this outside alphabeta?
			if(moves != 0 && capture == 1 && depth == maxdepth && oneCapture(b, from, moves)) {
				moveFrom = from;
				moveTo = recursiveTo;
				break;
			}
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
	unsigned int to = 0x0u;
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

bool oneCapture(board b, unsigned int from, unsigned int moves) {
	int bits = countBits(moves);

	while(bits == 1) {
		move(b, from, moves);
		from = moves;
		moves = getCaptureMoves(b, from);
		bits = countBits(moves);
	}
	recursiveTo = from;

	return bits == 0;
}
