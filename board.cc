#include <iostream>
#include <cmath>
#include <vector>
#include <queue>
#include "board.h"

namespace checkers {

  Board::Board() :
    black(0x0u),
    white(0x0u),
    kings(0x0u),
    player(BLACK)
  {}

  Board::~Board() {}

  void Board::createBoard() {
    black = 0x00000FFFu;
    white = 0xFFF00000u;
    kings = 0x0u;
    player = BLACK;
  }

  unsigned int Board::getCaptureMoves(unsigned int piece) {
    unsigned int moves = 0x0u;

    if(piece & white) {         // white man
      moves =   (up_left((up_left(piece) & black)) & (~(black|white))) |
        (up_right((up_right(piece) & black)) & (~(black|white)));
      if((piece & kings)) {     // white king
        moves |=        (down_left((down_left(piece) & black)) & (~(black|white))) |
          (down_right((down_right(piece) & black)) & (~(black|white)));
      }
    }
    else if(piece & black) {    // black man
      moves =   (down_left((down_left(piece) & white)) & (~(black|white))) |
        (down_right((down_right(piece) & white)) & (~(black|white)));
      if((piece & kings)) {     // black pimp
        moves |=        (up_left((up_left(piece) & white)) & (~(black|white))) |
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

  bool Board::endOfGame() {
      return ( getJumpPieces() | getMovePieces() ) == 0;
  }

  void Board::move(unsigned int from, unsigned int to) {
    // regular move
    unsigned int mask = player;
    --mask;                                             // 1-1=0, 0-1=2^32
    black &= ~from;
    black |= to & (~mask);
    white &= ~from;
    white |= to & mask;

    mask = ((from & kings) == 0);
    --mask;
    kings &= ~from;
    kings |= to & mask;

    // capture
    mask  = (up_left(from) & down_right(to));
    mask |= (up_right(from) & down_left(to));
    mask |= (down_left(from) & up_right(to));
    mask |= (down_right(from) & up_left(to));
    mask = ~mask;

    white &= mask;
    black &= mask;
    kings &= mask;
  }

  int Board::validateMove(unsigned int from, unsigned int to) {
    unsigned int moves = 0x0u;
    unsigned int men = 0x0u;

    if(getJumpPieces() != 0x0u)
        return validateCapture(from, to);

    player == WHITE ? men = white : men = black;
    moves = getMoves(from);
    if(((moves & to) != 0) && ((men & from) != 0))
      return 0;
    else return -1;
  }

  int Board::validateCapture(unsigned int from, unsigned int to) {
    unsigned int moves = getCaptureMoves(from);
    Board newboard;
    int result = -1;

    if(from == to)
      return -1;
    if((moves & to) != 0) {
      newboard = *this;
      newboard.move(from, to);
      if((newboard.getCaptureMoves(to)) == 0) {
        return 0;
      } else {
        return -2;
      }
    }
    while(moves != 0) {
      unsigned int nextto = (moves & (moves-1)) ^ moves;
      moves &= moves-1;
      newboard = *this;
      newboard.move(from, nextto);
		result = newboard.validateCapture(nextto, to);
		if(result == 0) {
        return 0;
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
    player == WHITE ? player = BLACK : player = WHITE;
  }

  inline bool Board::empty(unsigned int piece) {
    return (piece & (white|black)) == 0;
  }

  void Board::updateKings() {
    kings |= (white & 0x0000000Fu);
    kings |= (black & 0xF0000000u);
  }

	/**
	 * TODO: remove if-statement
	 */
	unsigned int Board::getJumpPieces() {
		unsigned int pieces = 0x0u;

		if(player == WHITE) {
			pieces |= down_right(down_right(up_left((up_left(white) & black)) & ~(black | white)));
			pieces |= down_left(down_left(up_right((up_right(white) & black)) & ~(black | white)));

			pieces |= up_right(up_right(down_left((down_left(white & kings) & black)) & ~(black | white)));
			pieces |= up_left(up_left(down_right((down_right(white & kings) & black)) & ~(black | white)));
		} else {
			pieces |= up_right(up_right(down_left((down_left(black) & white)) & ~(black | white)));
			pieces |= up_left(up_left(down_right((down_right(black) & white)) & ~(black | white)));

			pieces |= down_right(down_right(up_left((up_left(black & kings) & white)) & ~(black | white)));
			pieces |= down_left(down_left(up_right((up_right(black & kings) & white)) & ~(black | white)));
		}

		return pieces;
	}

	/**
	 * TODO: remove if-statement
	 */
	unsigned int Board::getMovePieces() {
		unsigned int pieces = 0x0u;

		if(player == WHITE) {
			pieces |= down_right((up_left(white) & ~(black | white)));
			pieces |= down_left((up_right(white) & ~(black | white)));
			
			pieces |= up_right((down_left(white & kings) & ~(black | white)));
			pieces |= up_left((down_right(white & kings) & ~(black | white)));
		} else {
			pieces |= up_right((down_left(black) & ~(black | white)));
			pieces |= up_left((down_right(black) & ~(black | white)));

			pieces |= down_right((up_left(black & kings) & ~(black | white)));
			pieces |= down_left((up_right(black & kings) & ~(black | white)));
		}

		return pieces;
	}
	
	bool Board::operator==(const Board& b) {
		return player == b.player && kings == b.kings &&
				black == b.black && white == b.white;
	}
}
