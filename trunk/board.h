#ifndef BOARD_H
#define BOARD_H

#include <queue>

#define WHITE 0x0
#define BLACK 0x1

namespace checkers {

	class Board {

	public:
		Board();
		~Board();
		unsigned int black;
		unsigned int white;
		unsigned int kings;
		bool player; // WHITE=0, BLACK=1

		static int bits_in_char [256];		// för bit-count

		void createBoard();
		unsigned int getCaptureMoves(unsigned int piece);
		unsigned int getMoves(unsigned int piece);
		bool endOfGame();
		void move(unsigned int from, unsigned int to);

		unsigned int getCaptureBit(unsigned int from, unsigned int to);

		void changePlayer();

		int countBits(unsigned int b);

		int countBits2(unsigned int b);

		void compute_bits_in_char();

		inline bool empty(unsigned int piece);

		bool validateMove(unsigned int from, unsigned int to);
		bool validateCapture(unsigned int from, unsigned int to);

		void updateKings();

		std::queue<unsigned int> getSortedMoveList();

		unsigned int getJumpPieces();
		unsigned int getMovePieces();
	private:
		inline unsigned int up_left(unsigned const int& piece);
		inline unsigned int up_right(unsigned const int& piece);
		inline unsigned int down_left(unsigned const int& piece);
		inline unsigned int down_right(unsigned const int& piece);
		
		static const unsigned int lg2[5];
	};
}

#endif
