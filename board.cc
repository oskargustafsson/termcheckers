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

	if(b.player == WHITE) {
		tmp = up_left(piece);
		if((tmp & b.black) != 0) {
			tmp = up_left(tmp);
			moves |= recursiveCapture(b, piece, tmp);
		}
		tmp = up_right(piece);
		if((tmp & b.black) != 0) {
			tmp = up_right(tmp);
			moves |= recursiveCapture(b, piece, tmp);
		}
	}
	if(b.player == BLACK) {
		tmp = down_left(piece);
		if((tmp & b.black) != 0) {
			tmp = down_left(tmp);
			moves |= recursiveCapture(b, piece, tmp);
		}
		tmp = down_right(piece);
		if((tmp & b.black) != 0) {
			tmp = down_right(tmp);
			moves |= recursiveCapture(b, piece, tmp);
		}
	}
	return moves;
}

unsigned int recursiveCapture(board b, unsigned int piece, unsigned int newpiece) {
	unsigned int moves = 0x0;
	if((newpiece & (b.black | b.white)) == 0) {
		move(b, piece, newpiece);
		if((moves = getCaptureMoves(b, piece)) != 0) {
			return moves;
		} else {
			return newpiece;
		}
	}
	return moves;
}

unsigned int getMoves(const board& b, unsigned int piece) {
	unsigned int moves = 0x0;

	if((piece & b.kings) != 0) { // if king
		moves = moves | up_left(piece);
		moves = moves | up_right(piece);
		moves = moves | down_left(piece);
		moves = moves | down_right(piece);
	} else if(b.player == WHITE) {
		moves = moves | up_left(piece);
		moves = moves | up_right(piece);
	} else {
		moves = moves | down_left(piece);
		moves = moves | down_right(piece);
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
	if(	(piece >= 0x100 && piece <= 0x800) ||
		(piece >= 0x10000 && piece <= 0x80000) ||
		(piece >= 0x1000000 && piece <= 0x8000000)
	  ) {
		return piece>>4;
	}
	if(	(piece > 0x10 && piece <= 0x80) ||
		(piece > 0x1000 && piece <= 0x8000) ||
		(piece > 0x100000 && piece <= 0x800000) ||
		(piece > 0x10000000 && piece <= 0x80000000)
	  ) {
		return piece>>5;
	}
	return 0x0;
}

unsigned int up_right(unsigned const int& piece) {
	if(	(piece > 0x8 && piece <= 0x80) ||
		(piece > 0x800 && piece <= 0x8000) ||
		(piece > 0x80000 && piece <= 0x800000) ||
		(piece > 0x8000000 && piece <= 0x80000000)
	  ) {
		return piece>>4;
	}
	if(	(piece > 0x80 && piece < 0x800) ||
		(piece > 0x8000 && piece < 0x80000) ||
		(piece > 0x800000 && piece < 0x8000000)
	  ) {
		return piece>>3;
	}
	return 0x0;
}

unsigned int down_left(unsigned const int& piece) {
	if(	(piece < 0x10) ||
		(piece >= 0x100 && piece < 0x1000) ||
		(piece >= 0x10000 && piece < 0x100000) ||
		(piece >= 0x1000000 && piece < 0x10000000)
	  ) {
		return piece<<4;
	}
	if(	(piece > 0x10 && piece <= 0x100) ||
		(piece > 0x1000 && piece <= 0x10000) ||
		(piece > 0x100000 && piece <= 0x1000000)
	  ) {
		return piece<<3;
	}
	return 0x0;
}

unsigned int down_right(unsigned const int& piece) {
	if(	(piece < 0x8) ||
		(piece > 0x80 && piece < 0x800) ||
		(piece > 0x8000 && piece < 0x80000) ||
		(piece > 0x800000 && piece < 0x8000000)
	  ) {
		return piece<<5;
	}
	if(	(piece > 0x8 && piece <= 0x80) ||
		(piece > 0x800 && piece <= 0x8000) ||
		(piece > 0x80000 && piece <= 0x800000)
	  ) {
		return piece<<4;
	}
	return 0x0;
}

/**
 * Returns true if there are no possible moves for current player
 */
bool endOfGame(const board& b) {
	if(b.player == WHITE) {
		for(int i=0; i<32; i++) {
			if((0x1 & (b.white>>i)) != 0) {
				if(getMoves(b, 0x1<<i) != 0x0) {
					return false;
				}
			}
		}
	} else if(b.player == BLACK) {
		for(int i=0; i<32; i++) {
			if((0x1 & (b.black>>i)) != 0) {
				if(getMoves(b, 0x1<<i) != 0x0) {
					return false;
				}
			}
		}
	}
	return true;
}

bool move(board& b, unsigned int from, unsigned int to) {
	int moves = 0x0;

	if(b.player == WHITE) {
		if((b.white & from) != 0) {
			moves = getMoves(b, from);
			if((moves & to) != 0) {
				b.white &= ~from;
				b.white |= to;

				if((from & b.kings) != 0) {
					b.kings &= ~from;
					b.kings |= to;
				}
				changePlayer(b);
				return true;
			}
		}
	} else if(b.player == BLACK) {
		if((b.black & from) != 0) {
			moves = getMoves(b, from);
			if((moves & to) != 0) {
				b.black &= ~from;
				b.black |= to;

				if((from & b.kings) != 0) {
					b.kings &= ~from;
					b.kings |= to;
				}
				changePlayer(b);
				return true;
			}
		}
	}

	return false;
}

void changePlayer(board& b) {
	if(b.player == WHITE) {
		b.player = BLACK;
	} else {
		b.player = WHITE;
	}
}
