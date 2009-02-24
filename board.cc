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
      moves = up_left(up_left(piece) & black) | up_right(up_right(piece) & black);
      if((piece & kings)) {     // white king
        moves |= down_left(down_left(piece) & black) | down_right(down_right(piece) & black);
      }
    }
    else if(piece & black) {    // black man
      moves = down_left(down_left(piece) & white) | down_right(down_right(piece) & white);
      if((piece & kings)) {     // black pimp
        moves |= up_left(up_left(piece) & white) | up_right(up_right(piece) & white);
      }
    }
    return moves & (~(black|white));
  }

        unsigned int Board::getCaptureMoves() {
                unsigned int moves = 0x0u;
                if(player == BLACK) {
                        moves = down_left(down_left(black) & white) |
                                        down_right(down_right(black) & white) |
                                        up_left(up_left(black&kings) & white) |
                                up_right(up_right(black&kings) & white);
                }
                else {
                        moves = up_left(up_left(white) & black) |
                                        up_right(up_right(white) & black) |
                                        down_left(down_left(white&kings) & black) |
                                down_right(down_right(white&kings) & black);
                }
                return moves & (~(black|white));
        }

  unsigned int Board::getMoves(unsigned int piece) {
        unsigned int allmask = ~(black|white);
    if((piece & kings) != 0) {
      return (up_left(piece) | up_right(piece) | down_left(piece) | down_right(piece)) & allmask;
    } else if((piece & white) != 0) {
      return (up_left(piece) | up_right(piece)) & allmask;
    } else if((piece & black) != 0) {
      return (down_left(piece) | down_right(piece))  & allmask;
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

  inline unsigned int Board::up(unsigned const int& piece) {
    return (piece & 0xFFFFFF00)>>8;
  }
  inline unsigned int Board::down(unsigned const int& piece) {
    return (piece & 0x00FFFFFF)<<8;
  }
  inline unsigned int Board::left(unsigned const int& piece) {
    return (piece & 0xEEEEEEEE)>>1;
  }
  inline unsigned int Board::right(unsigned const int& piece) {
    return (piece & 0x77777777)<<1;
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

  int Board::validateMove(std::vector<unsigned int> movements)
  {
          unsigned int moves = 0x0u;
          unsigned int men = 0x0u;

          unsigned int from = movements[0];
          unsigned int to = movements[1];

          if(movements.size() < 2)
                  return -3;
          if(getJumpPieces() != 0x0u)
                  return validateCapture(movements, 0, from);
          if(movements.size() != 2)
                  return -1;

          player == WHITE ? men = white : men = black;
          moves = getMoves(from);
          if(((moves & to) != 0) && ((men & from) != 0))
                  return 0;
          else return -1;
  }

  int Board::validateCapture(std::vector<unsigned int> movements, unsigned int pos, unsigned int from) {
          unsigned int moves = getCaptureMoves(from);
          int result = -1;

          if(movements.size()-1 == pos)
          {
                  if(moves != 0)
                          return -2;
                  else
                          return 0;
          }

          unsigned int to = movements[pos+1];

          if((moves & to) != 0)
          {
                  Board newboard = *this;
                  newboard.move(from, to);
                  return newboard.validateCapture(movements, pos+1, to);
          }
          while(moves != 0)
          {
                  to = (moves & (moves-1)) ^ moves;
                  moves &= moves-1;
                  Board newboard = *this;
                  newboard.move(from, to);
                  result = newboard.validateCapture(movements, pos, to);
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
    unsigned int allMask = ~(black | white);

    if(player == WHITE) {
      pieces |= down_right(down_right(up_left((up_left(white) & black)) & allMask));
      pieces |= down_left(down_left(up_right((up_right(white) & black)) & allMask));

      pieces |= up_right(up_right(down_left((down_left(white & kings) & black)) & allMask));
      pieces |= up_left(up_left(down_right((down_right(white & kings) & black)) & allMask));
    } else {
      pieces |= up_right(up_right(down_left((down_left(black) & white)) & allMask));
      pieces |= up_left(up_left(down_right((down_right(black) & white)) & allMask));

      pieces |= down_right(down_right(up_left((up_left(black & kings) & white)) & allMask));
      pieces |= down_left(down_left(up_right((up_right(black & kings) & white)) & allMask));
    }

    return pieces;
  }

        /**
         * TODO: remove if-statement
         */
        unsigned int Board::getMovePieces() {
                unsigned int pieces = 0x0u;
                unsigned int allMask = ~(black | white);

                if(player == WHITE) {
                        pieces |= down_right((up_left(white) & allMask));
                        pieces |= down_left((up_right(white) & allMask));

                        pieces |= up_right((down_left(white & kings) & allMask));
                        pieces |= up_left((down_right(white & kings) & allMask));
                } else {
                        pieces |= up_right((down_left(black) & allMask));
                        pieces |= up_left((down_right(black) & allMask));

                        pieces |= down_right((up_left(black & kings) & allMask));
                        pieces |= down_left((up_right(black & kings) & allMask));
                }

                return pieces;
        }

        bool Board::operator==(const Board& b) {
                return player == b.player && kings == b.kings &&
                                black == b.black && white == b.white;
        }

		unsigned int Board::box(unsigned int pieces) {
			return up(pieces) |
					up_left(up_left(pieces)) |
					left(pieces) |
					down_left(down_left(pieces)) |
					down(pieces) |
					down_right(down_right(pieces)) |
					right(pieces) |
					up_right(up_right(pieces));
		}
			
}
