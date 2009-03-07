#ifndef BOARD_H
#define BOARD_H

#include <vector>

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

		bool load(char* file);
		void new_board();
		unsigned int getCaptureMoves(unsigned int piece);
		unsigned int getCaptureMoves();
		unsigned int getMoves(unsigned int piece);
		bool endOfGame();
		void move(unsigned int from, unsigned int to);

		unsigned int getCaptureBit(unsigned int from, unsigned int to);

		void changePlayer();

		inline bool empty(unsigned int piece);

		int validateMove(std::vector<unsigned int> movements);
		int validateCapture(std::vector<unsigned int> movements, unsigned int pos, unsigned int from);

		void updateKings();

		unsigned int getJumpPieces();
		unsigned int getMovePieces();
		
		bool operator==(const Board&);
		unsigned int box(unsigned int pieces);
	private:
		inline unsigned int up_left(unsigned const int& piece);
		inline unsigned int up_right(unsigned const int& piece);
		inline unsigned int down_left(unsigned const int& piece);
		inline unsigned int down_right(unsigned const int& piece);
		inline unsigned int up(unsigned const int& piece);
		inline unsigned int right(unsigned const int& piece);
		inline unsigned int left(unsigned const int& piece);
		inline unsigned int down(unsigned const int& piece);
	};
}

#endif
