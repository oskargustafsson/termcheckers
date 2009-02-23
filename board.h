#ifndef BOARD_H
#define BOARD_H

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

		void createBoard();
		unsigned int getCaptureMoves(unsigned int piece);
		unsigned int getMoves(unsigned int piece);
		bool endOfGame();
		void move(unsigned int from, unsigned int to);

		unsigned int getCaptureBit(unsigned int from, unsigned int to);

		void changePlayer();

		inline bool empty(unsigned int piece);

		int validateMove(unsigned int from, unsigned int to);
		int validateCapture(unsigned int from, unsigned int to);

		void updateKings();

		unsigned int getJumpPieces();
		unsigned int getMovePieces();
		
		bool operator==(const Board&);
	private:
		inline unsigned int up_left(unsigned const int& piece);
		inline unsigned int up_right(unsigned const int& piece);
		inline unsigned int down_left(unsigned const int& piece);
		inline unsigned int down_right(unsigned const int& piece);
	};
}

#endif
