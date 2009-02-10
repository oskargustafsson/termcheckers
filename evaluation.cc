#include "evaluation.h"
#include "board.h"

int evaluate(board b) {
	int value = 0;

	for(int i=0x1; i != 0; i = (i<<1)) {
		if((b.white & i) != 0) {
			if(getMoves(b, i) > 0) {
				if(b.player == WHITE) {
					value += 1;
				} else {
					value -= 1;
				}
			}
		}
		if((b.black & i) != 0) {
			if(getMoves(b, i) > 0) {
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
