#include "board.h"

board createBoard() {
	board b;
	b.black = 0x00000FFF;
	b.white = 0xFFF00000;
	b.kings = 0x0;
	b.player = BLACK;
	return b;
}

unsigned int getCaptureMoves(const board& b, unsigned int piece) {
	unsigned int moves = 0x0;
	unsigned int tmp = 0x0;

	if((piece & b.white) != 0) { // if white
		tmp = up_left(piece);
		if((tmp & b.black) != 0) {
			tmp = up_left(tmp);
			if(empty(b, tmp)) {
				moves |= tmp;
			}
		}
		tmp = up_right(piece);
		if((tmp & b.black) != 0) {
			tmp = up_right(tmp);
			if(empty(b, tmp)) {
				moves |= tmp;
			}
		}
		if((piece & b.kings) != 0) { // if white king
			tmp = down_left(piece);
			if((tmp & b.black) != 0) {
				tmp = down_left(tmp);
				if(empty(b, tmp)) {
					moves |= tmp;
				}
			}
			tmp = down_right(piece);
			if((tmp & b.black) != 0) {
				tmp = down_right(tmp);
				if(empty(b, tmp)) {
					moves |= tmp;
				}
			}
		}
	}
	if((piece & b.black) != 0) { // if black
		tmp = down_left(piece);
		if((tmp & b.white) != 0) {
			tmp = down_left(tmp);
			if(empty(b, tmp)) {
				moves |= tmp;
			}
		}
		tmp = down_right(piece);
		if((tmp & b.white) != 0) {
			tmp = down_right(tmp);
			if(empty(b, tmp)) {
				moves |= tmp;
			}
		}
		if((piece & b.kings) != 0) { // if black king
			tmp = up_left(piece);
			if((tmp & b.white) != 0) {
				tmp = up_left(tmp);
				if(empty(b, tmp)) {
					moves |= tmp;
				}
			}
			tmp = up_right(piece);
			if((tmp & b.white) != 0) {
				tmp = up_right(piece);
				if(empty(b, piece)) {
					moves |= tmp;
				}
			}
		}
	}
	return moves;
}

unsigned int getMoves(const board& b, unsigned int piece) {
	unsigned int moves = 0x0;

	if((piece & b.kings) != 0) {
		moves |= up_left(piece);
		moves |= up_right(piece);
		moves |= down_left(piece);
		moves |= down_right(piece);
	} else if((piece & b.white) != 0) {
		moves |= up_left(piece);
		moves |= up_right(piece);
	} else if((piece & b.black) != 0) {
		moves |= down_left(piece);
		moves |= down_right(piece);
	}

	// remove occupied spaces
	moves = moves & ~(b.black|b.white);

	return moves;
}


/**
 * up_left, up_right, down_left, down_right
 * returns the bitmap after piece move one step
 */
unsigned int up_left(unsigned const int& piece) {
	if((piece & 0x0F0F0F00) != 0)
		return piece>>4;
	if((piece & 0xE0E0E0E0) != 0)
		return piece>>5;
	return 0x0;
}

unsigned int up_right(unsigned const int& piece) {
	if((piece & 0xF0F0F0F0) != 0)
		return piece>>4;
	if((piece & 0x07070700) != 0)
		return piece>>3;
	return 0x0;
}

unsigned int down_left(unsigned const int& piece) {
	if((piece & 0x0F0F0F0F) != 0)
		return piece<<4;
	if((piece & 0x00E0E0E0) != 0)
		return piece<<3;
	return 0x0;
}

unsigned int down_right(unsigned const int& piece) {
	if((piece & 0x00F0F0F0) != 0)
		return piece<<4;
	if((piece & 0x07070707) != 0)
		return piece<<5;
	return 0x0;
}

/**
 * Returns true if there are no possible moves for current player
 */
bool endOfGame(const board& b) {
	for(int i=0x1; i != 0; i = (i<<1)) {
		if((((b.player == BLACK) && ((i & b.black) != 0)) || (b.player == WHITE) && ((i & b.white) != 0))) {
			if(getMoves(b, i) != 0x0)
				return false;
			if(getCaptureMoves(b, i) != 0x0)
				return false;
		}
	}
	return true;
}

/**
 * Dosn't check anything! any move is possible
 */
void move(board& b, unsigned int from, unsigned int to) {
	int maskrows = 0xF0F0F0F0;

	if((b.white & from) != 0) {
		b.white &= ~from;
		b.white |= to;
		if((to & 0x0000000F) != 0) {
			b.kings |= to;
		}

		if (((maskrows & from) == 0) == ((maskrows & to) == 0)) { // capture move
			b.black &= ~getCaptureBit(from, to);
		}
	}
	if((b.black & from) != 0) {
		b.black &= ~from;
		b.black |= to;
		if((to & 0xF0000000) != 0) {
			b.kings |= to;
		}

		if (((maskrows & from) == 0) == ((maskrows & to) == 0)) { // capture move
			b.white &= ~getCaptureBit(from, to);
		}
	}
	if((from & b.kings) != 0) {
		b.kings &= ~from;
		b.kings |= to;
	}
}

/**
 * get the position between two squares
 */
unsigned int getCaptureBit(unsigned int from, unsigned int to) {
	if(from < to) {
		if(down_left(down_left(from)) == to) {
			return down_left(from);
		} else if(down_right(down_right(from)) == to) {
			return down_right(from);
		}
	} else {
		if(up_left(up_left(from)) == to) {
			return up_left(from);
		} else if(up_right(up_right(from)) == to) {
			return up_right(from);
		}
	}
	return 0x0;
}

void changePlayer(board& b) {
	if(b.player == WHITE) {
		b.player = BLACK;
	} else {
		b.player = WHITE;
	}
}

int countBits(unsigned int board) {
	return __builtin_popcount (board);
}

bool empty(const board& b, unsigned int piece) {
	return (piece & (b.white|b.black)) == 0;
}
