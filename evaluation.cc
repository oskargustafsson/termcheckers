#include "evaluation.h"
#include "board.h"

int evaluate(board b) {
	int value = 0;

	for(int i=0x1; i != 0; i = (i<<1)) {
		if((b.white_men & i) != 0) {
			if(getMoves(b, i, false) > 0) {
				if(b.player == WHITE) {
					value += 1;
				} else {
					value -= 1;
				}
			}
		}
		if((b.black_men & i) != 0) {
			if(getMoves(b, i, false) > 0) {
				if(b.player == BLACK) {
					value += 1;
				} else {
					value -= 1;
				}
			}
		}
	}
	return value;
}
