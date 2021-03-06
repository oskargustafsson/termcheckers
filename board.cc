#include <fstream>
#include <iostream>
#include <cmath>
#include <vector>
#include <queue>
#include <algorithm>
#include "board.h"

namespace checkers {

	Board::Board() :
		black(0x00000FFFu),
		white(0xFFF00000u),
		kings(0x0u),
		player(BLACK)
	{}

	Board::~Board() {}

	void Board::new_board()
	{
		black = 0x00000FFFu;
		white = 0xFFF00000u;
		kings = 0x0u;
		player = BLACK;
	}

	bool Board::load(const char* file)
	{
		std::ifstream is;
		is.open(file);
		unsigned int piece = 0x1;

		if(is != NULL) {
			char ch;
			black = 0;
			white = 0;
			kings = 0;
			while(is.get(ch) != NULL) {
				if(ch == 'b') {
					if(piece == 0) {
						player = BLACK;
					}
					black |= piece;
					white &= ~piece;
					kings &= ~piece;
					piece = piece<<1;
				} else if(ch == 'B') {
					black |= piece;
					kings |= piece;
					white &= ~piece;
					piece = piece<<1;
				} else if(ch == 'w') {
					if(piece == 0) {
						player = WHITE;
					}
					white |= piece;
					black &= ~piece;
					kings &= ~piece;
					piece = piece<<1;
				} else if(ch == 'W') {
					white |= piece;
					kings |= piece;
					black &= ~piece;
					piece = piece<<1;
				} else if(ch == '.') {
					black &= ~piece;
					white &= ~piece;
					kings &= ~piece;
					piece = piece<<1;
				}
			}
			is.close();
			return true;
		}
		return false;
	}

	bool Board::save(const char* file)
	{
		std::ofstream os;
		os.open(file);
		if(os != NULL)
		{
			unsigned int piece = 1;
			while(piece != 0)
			{
				if(piece & black & kings)
					os << 'B';
				else if(piece & black)
					os << 'b';
				else if(piece & white & kings)
					os << 'W';
				else if(piece & white)
					os << 'w';
				else
					os << '.';
				piece = piece<<1;
			}
			player == BLACK ? os << 'b' : os << 'w';
			os.close();
			return true;
		}
		return false;
	}

	unsigned int Board::getCaptureMoves(unsigned int piece)
	{
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

	int Board::validateMove(std::vector<unsigned int>& movements)
	{
		unsigned int moves = 0x0u;
		unsigned int men = 0x0u;

		std::vector<unsigned int> tmp;
		unsigned int from = movements[0];
		unsigned int to = movements[1];

		if(movements.size() < 2)
			return -3;
		if(getJumpPieces() != 0x0u)
		{
			int result = validateCapture(movements, 0, from, tmp);
			if(result == 0)
			{
				std::reverse(tmp.begin(), tmp.end());
				movements = tmp;
			}
			return result;
		}
		if(movements.size() != 2)
			return -1;

		player == WHITE ? men = white : men = black;
		moves = getMoves(from);
		if(((moves & to) != 0) && ((men & from) != 0))
			return 0;
		else return -1;
	}

	int Board::validateCapture(std::vector<unsigned int> movements, unsigned int pos, unsigned int from, std::vector<unsigned int>& tmp) {
		unsigned int moves = getCaptureMoves(from);
		int result = -1;

		if(movements.size()-1 == pos)
		{
			if(moves != 0)
				return -2;
			else
			{
				tmp.push_back(from);
				return 0;
			}
		}

		unsigned int to = movements[pos+1];

		if((moves & to) != 0)
		{
			Board newboard = *this;
			newboard.move(from, to);
			result = newboard.validateCapture(movements, pos+1, to, tmp);
			if(result == 0)
			{
				tmp.push_back(from);
			}
			return result;
		}
		while(moves != 0)
		{
			to = (moves & (moves-1)) ^ moves;
			moves &= moves-1;
			Board newboard = *this;
			newboard.move(from, to);
			result = newboard.validateCapture(movements, pos, to, tmp);
			if(result == 0)
			{
				tmp.push_back(from);
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
