#include <iostream>
#include "board.h"
#include "gui.h"
// ui.h bara för debug
namespace checkers {

	Board::Board() :
		black(0x0u),
		      white(0x0u),
		      kings(0x0u),
		      player(BLACK),
		      newKing(false)
		      {}

	Board::~Board() {}

	Board& Board::operator=(Board& b) {
		b.black = black;
		b.white = white;
		b.kings = kings;
		b.player = player;
		b.newKing = newKing;
		return b;
	}

	void Board::createBoard() {
		black = 0x00000FFFu;
		white = 0xFFF00000u;
		kings = 0x0u;
		player = BLACK;
		newKing = false;
	}

	unsigned int Board::getCaptureMoves(unsigned int piece) {
		unsigned int moves = 0x0u;

		if(piece & white) {		// white man
			moves =	(up_left((up_left(piece) & black)) & (~(black|white))) |
				(up_right((up_right(piece) & black)) & (~(black|white)));
			if((piece & kings) && !newKing) {	// white king
				moves |=	(down_left((down_left(piece) & black)) & (~(black|white))) |
					(down_right((down_right(piece) & black)) & (~(black|white)));
			}
		}
		else if(piece & black) {	// white man
			moves =	(down_left((down_left(piece) & white)) & (~(black|white))) |
				(down_right((down_right(piece) & white)) & (~(black|white)));
			if((piece & kings) && !newKing) {	// black pimp
				moves |=	(up_left((up_left(piece) & white)) & (~(black|white))) |
					(up_right((up_right(piece) & white)) & (~(black|white)));
			}
		}
		return moves;
	}

	unsigned int Board::getMoves(unsigned int piece) {
		if((piece & kings) != 0) {
			return (up_left(piece) | up_right(piece) | down_left(piece) | down_right(piece)) & (~(black|white));
		} else if((piece & white) != 0) {
			return (up_left(piece) | up_right(piece)) & (~(black|white));
		} else if((piece & black) != 0) {
			return (down_left(piece) | down_right(piece))  & (~(black|white));
		}
		return 0x0u;
	}

	/**
	 * up_left, up_right, down_left, down_right
	 * returns the bitmap after piece move one step
	 */
	inline unsigned int Board::up_left(unsigned const int& piece) {
		return (((piece & 0x0F0F0F00)>>4) | ((piece & 0xE0E0E0E0)>>5));
	}

	inline unsigned int Board::up_right(unsigned const int& piece) {
		return (((piece & 0xF0F0F0F0)>>4) | ((piece & 0x07070700)>>3));
	}

	inline unsigned int Board::down_left(unsigned const int& piece) {
		return (((piece & 0x0F0F0F0F)<<4) | ((piece & 0x00E0E0E0)<<3));
	}

	inline unsigned int Board::down_right(unsigned const int& piece) {
		return (((piece & 0x00F0F0F0)<<4) | ((piece & 0x07070707)<<5));
	}

	/**
	 * Returns true if there are no possible moves for current player
	 * OPTIMIZE
	 */
	bool Board::endOfGame() {
		for(int i=0x1; i != 0; i = (i<<1)) {
			if(((player == BLACK) && ((i & black) != 0)) || ((player == WHITE) && ((i & white) != 0))) {
				if(getMoves(i) != 0x0u)
					return false;
				if(getCaptureMoves(i) != 0x0u)
					return false;
			}
		}
		return true;
	}

	/**
	 * Dosn't check anything! any move is possible
	 * only one jump!
	 * OPTIMIZE
	 */
	void Board::move(unsigned int from, unsigned int to) {
		unsigned int maskrows = 0xF0F0F0F0;
		newKing = false;

		//	printInt(getPossibleMoves(b));		//giltigt drag?
		//	printInt(getNextLevel(b));

		if((white & from) != 0) {
			white &= ~from;
			white |= to;
			if(((to & 0x0000000F) != 0) && ((kings & from) == 0)) {
				newKing = true;
				kings |= to;
			}

			if (((maskrows & from) == 0) == ((maskrows & to) == 0)) { // capture move
				black &= ~getCaptureBit(from, to);
				kings &= ~getCaptureBit(from, to);
			}
		}
		if((black & from) != 0) {
			black &= ~from;
			black |= to;
			if(((to & 0xF0000000) != 0) && ((kings & from) == 0)) {
				newKing = true;
				kings |= to;
			}

			if (((maskrows & from) == 0) == ((maskrows & to) == 0)) { // capture move
				white &= ~getCaptureBit(from, to);
				kings &= ~getCaptureBit(from, to);
			}
		}

		if((from & kings) != 0) {
			kings &= ~from;
			kings |= to;
		}
	}

	bool Board::validateMove(unsigned int from, unsigned int to) {
		unsigned int moves = 0x0u;
		unsigned int men = 0x0u;

		player == WHITE ? men = white : men = black;
		while(men != 0) {
			unsigned int tmp = (men & (men-1)) ^ men;
			men &= men-1;
			if(getCaptureMoves(tmp) != 0) {
				return validateCapture(from, to);
			}
		}

		player == WHITE ? men = white : men = black;
		moves = getMoves(from);
		return ((moves & to) != 0) && ((men & from) != 0);
	}

	bool Board::validateCapture(unsigned int from, unsigned int to) {
		unsigned int moves = getCaptureMoves(from);
		Board newboard;
		bool result = false;

		if((moves & to) != 0) {
			newboard = *this;
			newboard.move(from, to);
			if((newboard.getCaptureMoves(to)) == 0) {
				return true;
			} else {
				std::cout << "More captures possible!\n";
				return false;
			}
		} 
		while(moves != 0) {
			unsigned int nextto = (moves & (moves-1)) ^ moves;
			moves &= moves-1;
			newboard = *this;
			newboard.move(from, nextto);
			if(newboard.validateCapture(nextto, to)) {
				result = true;
			}
		}
		return result;
	}


	/**
	 * get the position between two squares
	 * OPTIMIZE
	 */
	unsigned int Board::getCaptureBit(unsigned int from, unsigned int to) {
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

	void Board::changePlayer() {
		newKing = false;
		player == WHITE ? player = BLACK : player = WHITE;
	}

	int Board::countBits(unsigned int board) {
		return __builtin_popcount (board);
	}

/*
	void Board::compute_bits_in_char()
	{
		unsigned int i ;    
		for (i = 0; i < 256; i++)
			bits_in_char [i] = countBits(i) ;
		return ;
	}

	int Board::countBits2(unsigned int n)
	{
		//kräver att bits_in_char är initierad

		return bits_in_char [ n        & 0xffu]
			+  bits_in_char [(n >>  8) & 0xffu]
			+  bits_in_char [(n >> 16) & 0xffu]
			+  bits_in_char [(n >> 24) & 0xffu] ;
	}

*/
	inline bool Board::empty(unsigned int piece) {
		return (piece & (white|black)) == 0;
	}
}
