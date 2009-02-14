#include <iostream>
#include "board.h"
#include "ui.h"
			// ui.h bara för debug

board createBoard() {
	board b;
	b.black = 0x00000FFF;
	b.white = 0xFFF00000;
	b.kings = 0x0u;
	b.player = BLACK;
	return b;
}

unsigned int getCaptureMoves(const board& b, unsigned int piece) {
	unsigned int moves = 0x0u;

	if(piece & b.white) {		// white man
		moves =	(up_left((up_left(piece) & b.black)) & (~(b.black|b.white))) |
				(up_right((up_right(piece) & b.black)) & (~(b.black|b.white)));
		if(piece & b.kings) {	// white king
			moves |=	(down_left((down_left(piece) & b.black)) & (~(b.black|b.white))) |
						(down_right((down_right(piece) & b.black)) & (~(b.black|b.white)));
		}
	}
	else if(piece & b.black) {	// white man
		moves =	(down_left((down_left(piece) & b.white)) & (~(b.black|b.white))) |
				(down_right((down_right(piece) & b.white)) & (~(b.black|b.white)));
		if(piece & b.kings) {	// black pimp
			moves |=	(up_left((up_left(piece) & b.white)) & (~(b.black|b.white))) |
						(up_right((up_right(piece) & b.white)) & (~(b.black|b.white)));
		}
	}
	return moves;
}

unsigned int getMoves(const board& b, unsigned int piece) {
	if((piece & b.kings) != 0) {
		return (up_left(piece) | up_right(piece) | down_left(piece) | down_right(piece)) & (~(b.black|b.white));
	} else if((piece & b.white) != 0) {
		return (up_left(piece) | up_right(piece)) & (~(b.black|b.white));
	} else if((piece & b.black) != 0) {
		return (down_left(piece) | down_right(piece))  & (~(b.black|b.white));
	}
	return 0x0u;
}


/**
 * up_left, up_right, down_left, down_right
 * returns the bitmap after piece move one step
 */
inline unsigned int up_left(unsigned const int& piece) {
	return (((piece & 0x0F0F0F00)>>4) | ((piece & 0xE0E0E0E0)>>5));
}

inline unsigned int up_right(unsigned const int& piece) {
	return (((piece & 0xF0F0F0F0)>>4) | ((piece & 0x07070700)>>3));
}

inline unsigned int down_left(unsigned const int& piece) {
	return (((piece & 0x0F0F0F0F)<<4) | ((piece & 0x00E0E0E0)<<3));
}

inline unsigned int down_right(unsigned const int& piece) {
	return (((piece & 0x00F0F0F0)<<4) | ((piece & 0x07070707)<<5));
}

/**
 * Returns true if there are no possible moves for current player
 * OPTIMIZE this run for every node in searchtree
 */
bool endOfGame(const board& b) {
	for(int i=0x1; i != 0; i = (i<<1)) {
		if(((b.player == BLACK) && ((i & b.black) != 0)) || ((b.player == WHITE) && ((i & b.white) != 0))) {
			if(getMoves(b, i) != 0x0u)
				return false;
			if(getCaptureMoves(b, i) != 0x0u)
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

//	printInt(getPossibleMoves(b));		//giltigt drag?
//	printInt(getNextLevel(b));

	if((b.white & from) != 0) {
		b.white &= ~from;
		b.white |= to;
		if((to & 0x0000000F) != 0) {
			b.kings |= to;
		}

		if (((maskrows & from) == 0) == ((maskrows & to) == 0)) { // capture move
			b.black &= ~getCaptureBit(from, to);
			b.kings &= ~getCaptureBit(from, to);
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
			b.kings &= ~getCaptureBit(from, to);
		}
	}
	if((from & b.kings) != 0) {
		b.kings &= ~from;
		b.kings |= to;
	}
}

bool validateMove(board& b, unsigned int from, unsigned int to) {
	unsigned int moves = 0x0u;
	unsigned int men = 0x0u;
	bool capture = false;

	b.player == WHITE ? men = b.white : men = b.black;
	while(men != 0) {
		unsigned int tmp = (men & (men-1)) ^ men;
		men &= men-1;
		if(getCaptureMoves(b, tmp) != 0) {
			capture = true;
			break;
		}
	}
	b.player == WHITE ? men = b.white : men = b.black;

	if(capture) {
		return validateCapture(b, from, to);
	}
	moves = getMoves(b, from);
	return ((moves & to) != 0) && ((men & from) != 0);
}

bool validateCapture(board& b, unsigned int from, unsigned int to) {
	unsigned int moves = getCaptureMoves(b, from);
	board newboard;
	bool result = false;
	
	if((moves & to) != 0) {
		newboard = b;
		move(newboard, from, to);
		if((getCaptureMoves(newboard, to)) == 0) {
			return true;
		} else {
			std::cout << "More captures possible!\n";
			return false;
		}
	} 
	while(moves != 0) {
		unsigned int nextto = (moves & (moves-1)) ^ moves;
		moves &= moves-1;
		newboard = b;
		move(newboard, from, nextto);
		if(validateCapture(newboard, nextto, to)) {
			result = true;
		}
	}
	return result;
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
	return 0x0u;
}

void changePlayer(board& b) {
	b.player == WHITE ? b.player = BLACK : b.player = WHITE;
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

inline bool empty(const board& b, unsigned int piece) {
	return (piece & (b.white|b.black)) == 0;
}
