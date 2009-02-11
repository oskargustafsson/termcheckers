#include <cmath>
#include "searchtree.h"
#include "evaluation.cc"
#include "board.h"

int alphabeta(board& b) {
	return alphabeta(b, DEPTH_TMP, -32000, 32000);
}

int alphabeta(board& b, int depth, int alpha, int beta) {
	int men;
	int moves;
	int tmp;
	bool betacutoff = false;
	int moveFrom = 0x0;
	int moveTo = 0x0;
	bool capture = false;

	if(endOfGame(b) || depth == 0) {
		if(currentplayer == BLACK) {
			return evaluate(b);
		} else {
			return -evaluate(b);
		}
	}

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
		if(getCaptureMoves(b, i) != 0) {
			capture = true;
			break;
		}
	}
	
	for(int i=0x1; i != 0; i = (i<<1)) {
		if((i & men) == 0)
			continue;

		if(capture) {
			moves = getCaptureMoves(b, i);
		} else {
			moves = getMoves(b, i);
		}

		if(moves != 0) {
			for(int j=0x1; j != 0; j = (j<<1)) {
				if((j & moves) == 0)
					continue;

				board nextboard = b;
				move(nextboard, i, j);
				changePlayer(nextboard);

				tmp = -alphabeta(nextboard, depth-1, -beta, -alpha);
				if(tmp > alpha) {
					alpha = tmp;
					moveFrom = i;
					moveTo = j;
	//				printf("\033[3%dm%d(%d) \033[0m", depth, depth, alpha);
	//			} else {
	//				printf("%d(%d) ", depth, alpha);
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
	}
	if(depth == DEPTH_TMP) {
		// The root node, make the best move
		makeMove(b, moveFrom, moveTo);
	}
	return alpha;
}
