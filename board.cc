#include "board.h"

board createBoard() {
	board b;
	b.black_men = 0x00000FFF;
	b.white_men = 0xFFF00000;
	b.black_kings = 0x0;
	b.white_kings = 0x0;
	b.player = BLACK;
	return b;
}

unsigned int getMoves(const board& b, unsigned int piece, bool king) {
	unsigned int moves = 0x0;
	unsigned int all = b.black_men | b.white_men | b.black_kings | b.white_kings;

	if(king) {
		moves = moves | up_left(piece);
		moves = moves | up_right(piece);
		moves = moves | down_left(piece);
		moves = moves | down_right(piece);
	} else {
		switch(b.player) {
			case WHITE:
				moves = moves | up_left(piece);
				moves = moves | up_right(piece);
				break;
			case BLACK:
				moves = moves | down_left(piece);
				moves = moves | down_right(piece);
				break;
		}
	}

	// remove occupied spaces
	moves = moves & ~all;

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
			if((0x1 & (b.white_men>>i)) != 0) {
				if(getMoves(b, 0x1<<i, false) != 0x0) {
					return false;
				}
			}
			if((0x1 & (b.white_kings>>i)) != 0) {
				if(getMoves(b, 0x1<<i, true) != 0x0) {
					return false;
				}
			}
		}
	} else if(b.player == BLACK) {
		for(int i=0; i<32; i++) {
			if((0x1 & (b.black_men>>i)) != 0) {
				if(getMoves(b, 0x1<<i, false) != 0x0) {
					return false;
				}
			}
			if((0x1 & (b.black_kings>>i)) != 0) {
				if(getMoves(b, 0x1<<i, true) != 0x0) {
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
		if((b.white_men & from) != 0) {
			moves = getMoves(b, from, false);
			if((moves & to) != 0) {
				b.white_men &= ~from;
				b.white_men |= to;
				return true;
			}
		} else if((b.white_kings & from) != 0) {
			moves = getMoves(b, from, true);
			if((moves & to) != 0) {
				b.white_kings &= ~from;
				b.white_kings |= to;
				return true;
			}
		}
	} else if(b.player == BLACK) {
		if((b.black_men & from) != 0) {
			moves = getMoves(b, from, false);
			if((moves & to) != 0) {
				b.black_men &= ~from;
				b.black_men |= to;
				return true;
			}
		} else if((b.black_kings & from) != 0) {
			moves = getMoves(b, from, true);
			if((moves & to) != 0) {
				b.black_kings &= ~from;
				b.black_kings |= to;
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
