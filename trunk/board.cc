#include "board.h"
#include "ui.h"
			// ui.h bara för debug

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
				tmp = up_right(tmp);
				if(empty(b, tmp)) {
					moves |= tmp;
				}
			}
		}
	}
	return moves;
}

unsigned int getRecursiveCaptureMoves(board b, unsigned int piece) {
	unsigned int moves = getCaptureMoves(b, piece);
	unsigned int capture = 0x0;
	if(moves == 0) {
		return piece;
	}
	while(moves != 0) {
		capture = (moves & (moves-1)) ^ moves;
		moves &= moves-1;
	}
	return 0x0;
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
		if(((b.player == BLACK) && ((i & b.black) != 0)) || ((b.player == WHITE) && ((i & b.white) != 0))) {
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
 * only one jump!
 */
void move(board& b, unsigned int from, unsigned int to) {
	unsigned int maskrows = 0xF0F0F0F0;

//	if((to & getPossibleMoves(b)) == 0) return;		//giltigt drag?

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

bool validateMove(board& b, unsigned int from, unsigned int to) {
	return true;
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

void compute_bits_in_char()
{
    unsigned int i ;    
    for (i = 0; i < 256; i++)
        bits_in_char [i] = countBits(i) ;
    return ;
}

int countBits2(unsigned int n)
{
    //kräver att bits_in_char är initierad
    
    return bits_in_char [ n        & 0xffu]
        +  bits_in_char [(n >>  8) & 0xffu]
        +  bits_in_char [(n >> 16) & 0xffu]
        +  bits_in_char [(n >> 24) & 0xffu] ;
}
/*
// ej kungar, än
unsigned int getPossibleMoves(board& b) {
	unsigned int allpieces = b.black|b.white;
	unsigned int pieces;

	if(b.player == BLACK) {
		pieces = b.black;
		printInt( (	((pieces & 0x0F0F0F0F)<<4) | ((pieces & 0x00E0E0E0)<<3) |
					((pieces & 0x00F0F0F0)<<4) | ((pieces & 0x07070707)<<5) ) & (~allpieces) );

	//	getPossibleCaptureMoves(b);
		return	( (	((pieces & 0x0F0F0F0F)<<4) | ((pieces & 0x00E0E0E0)<<3) |
					((pieces & 0x00F0F0F0)<<4) | ((pieces & 0x07070707)<<5) ) & (~allpieces) );
	}
	else {
		pieces = b.white;
		printInt( (	((pieces & 0x0F0F0F00)>>4) | ((pieces & 0xE0E0E0E0)>>5) |
					((pieces & 0xF0F0F0F0)>>4) | ((pieces & 0x07070700)>>3) ) & (~allpieces) );

		return	( (	((pieces & 0x0F0F0F00)>>4) | ((pieces & 0xE0E0E0E0)>>5) |
					((pieces & 0xF0F0F0F0)>>4) | ((pieces & 0x07070700)>>3) ) & (~allpieces) );
	}
}

unsigned int getPossibleCaptureMoves(board b) {
	unsigned int moves;
	if(moves = getNextLevel(b, b.black)) {
		b.black = moves;
		return getNextLevel(b, b.black);
	}
	else {
		return 0;
	}
	return 0;
}

unsigned int getNextLevel(board b, unsigned int moves) {
	if(b.player == BLACK) {
		return getPossibleMoves( (	((moves & 0x0F0F0F0F)<<4) | ((moves & 0x00E0E0E0)<<3) |
					((moves & 0x00F0F0F0)<<4) | ((moves & 0x07070707)<<5) ) & (b.white) );
	}
	else {
		return getPossibleMoves( (	((moves & 0x0F0F0F00)>>4) | ((moves & 0xE0E0E0E0)>>5) |
					((moves & 0xF0F0F0F0)>>4) | ((moves & 0x07070700)>>3) ) & (b.white) );
	}
}
*/
bool empty(const board& b, unsigned int piece) {
	return (piece & (b.white|b.black)) == 0;
}
