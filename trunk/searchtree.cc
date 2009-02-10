#include "searchtree.h"
#include "evaluation.cc"
#include "board.h"

int alphabeta(board& b) {
	return alphabeta(b, DEPTH, -32000, 32000);
}

int alphabeta(board& b, int depth, int alpha, int beta) {
	int kings;
	int men;
	int moves;
	int a = -32000;
	int tmp;
	board result;
	if(endOfGame(b) || depth == 0) {
		return evaluate(b);
	}

	if(b.player == WHITE) {
		men = b.white_men;
		kings = b.white_kings;
	} else {
		men = b.black_men;
		kings = b.black_kings;
	}
	
	for(int i=0x1; i != 0; i = (i<<1)) {
		if((i & men) == 0)
			continue;
		moves = getMoves(b, i, false);
		if(moves != 0) {
			for(int j=0x1; j != 0; j = (j<<1)) {
				if((j & moves) == 0)
					continue;

				board nextboard = b;
				move(nextboard, i, j);
				changePlayer(nextboard);

				tmp = -alphabeta(nextboard, depth-1, -alpha, -beta);
				if(tmp > a) {
					a = tmp;
					result = nextboard;
//					printf("\033[3%dm%d(%d) \033[0m", depth, depth, a);
//				} else {
//					printf("%d(%d) ", depth, a);
				}
			}
		}
	}
	if(depth == DEPTH) {
		b = result;
	}
	return a;
}
