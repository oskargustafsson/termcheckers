#include "searchtree.h"

board alphabeta(board& b) {
	return alphabeta(b, 3, -32000, 32000);
}

board alphabeta(board b, int depth, int alpha, int beta) {
	int kings;
	int men;
	int moves;
	if(b.player == WHITE) {
		men = b.white_men;
		kings = b.white_kings;
	} else {
		men = b.black_men;
		kings = b.black_kings;
	}
	
	for(int i=0; i<32; i++) {
		if(((0x1<<i) & men) != 0) {
			moves = getMoves(b, 0x1<<i, false);
			if(moves != 0) {
				for(int j=0; j<32; j++) {
					if(((0x1<<j) & moves) != 0) {
						move(b, 0x1<<i, 0x1<<j);
						return b;
					}
				}
			}
		}
	}
	return b;
}

int max(int a, int b) {
	if(b > a)
		return b;
	else
		return a;
}
