#include "board.h"

board createBoard() {
	board b;
	b.blackmen = 0xFFF00000;
	b.whitemen = 0x00000FFF;
	b.blackkings = 0x0;
	b.whitekings = 0x0;
	b.player = WHITE;
	return b;
}

unsigned int getMoves(const board& b, unsigned int piece, bool king) {
	unsigned int moves = 0x0;
	unsigned int all = b.blackmen | b.whitemen | b.blackkings | b.whitekings;

	if(king) {
		moves = moves | upleft(piece);
		moves = moves | upright(piece);
		moves = moves | downleft(piece);
		moves = moves | downright(piece);
	} else {
		switch(b.player) {
			case WHITE:
				moves = moves | downleft(piece);
				moves = moves | downright(piece);
				break;
			case BLACK:
				moves = moves | upleft(piece);
				moves = moves | upright(piece);
				break;
		}
	}

	/**
	 * IMPLEMENT: REMOVE MOVES TO OCCUPIED SPACES
	 * have tried "moves = moves & (!all)"
	 */

	return moves;
}

unsigned int upleft(unsigned const int& piece) {
	if(	(piece > 0x8 && piece < 0x100) ||
		(piece > 0x800 && piece < 0x10000) ||
		(piece > 0x80000 && piece < 0x1000000) ||
		(piece > 0x8000000 && piece < 0x80000001)
	  ) {
		return piece>>4;
	}
	if(	(piece > 0x100 && piece < 0x1000) ||
		(piece > 0x10000 && piece < 0x100000) ||
		(piece > 0x1000000 && piece < 0x10000000)
	  ) {
		return piece>>5;
	}
	return 0x0;
}

unsigned int upright(unsigned const int& piece) {
	if(	(piece > 0x8 && piece < 0x80) ||
		(piece > 0x800 && piece < 0x8000) ||
		(piece > 0x80000 && piece < 0x800000) ||
		(piece > 0x8000000 && piece < 0x80000000)
	  ) {
		return piece>>3;
	}
	if(	(piece > 0x80 && piece < 0x1000) ||
		(piece > 0x8000 && piece < 0x100000) ||
		(piece > 0x800000 && piece < 0x10000000)
	  ) {
		return piece>>4;
	}
	return 0x0;
}

unsigned int downleft(unsigned const int& piece) {
	if(	(piece > 0x1 && piece < 0x10) ||
		(piece > 0x100 && piece < 0x1000) ||
		(piece > 0x10000 && piece < 0x100000) ||
		(piece > 0x1000000 && piece < 0x10000000)
	  ) {
		return piece<<3;
	}
	if(	(piece > 0x8 && piece < 0x100) ||
		(piece > 0x800 && piece < 0x10000) ||
		(piece > 0x80000 && piece < 0x1000000)
	  ) {
		return piece<<4;
	}
	return 0x0;
}

unsigned int downright(unsigned const int& piece) {
	if(	(piece < 0x10) ||
		(piece > 0x80 && piece < 0x1000) ||
		(piece > 0x8000 && piece < 0x100000) ||
		(piece > 0x800000 && piece < 0x10000000)
	  ) {
		return piece<<4;
	}
	if(	(piece > 0x8 && piece < 0x80) ||
		(piece > 0x800 && piece < 0x8000) ||
		(piece > 0x80000 && piece < 0x800000)
	  ) {
		return piece<<5;
	}
	return 0x0;
}

