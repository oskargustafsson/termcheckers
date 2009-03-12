#ifndef BOARD_H
#define BOARD_H

#include <vector>

namespace checkers {

	enum PLAYER { WHITE, BLACK };

	class Board {

	public:
		Board();
		~Board();
		unsigned int black;
		unsigned int white;
		unsigned int kings;
		PLAYER player;

		bool load(const char* file);
		bool save(const char* file);
		void new_board();
		unsigned int getCaptureMoves(unsigned int piece);
		unsigned int getCaptureMoves();
		unsigned int getMoves(unsigned int piece);
		bool endOfGame();
		void move(unsigned int from, unsigned int to);

		unsigned int getCaptureBit(unsigned int from, unsigned int to);

		void changePlayer();

		inline bool empty(unsigned int piece);

		int validateMove(std::vector<unsigned int>& movements);
		int validateCapture(std::vector<unsigned int> movements, unsigned int pos, unsigned int from, std::vector<unsigned int>&);

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
