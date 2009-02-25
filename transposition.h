#ifndef TRANSPOSITION_H
#define TRANSPOSITION_H

//#define TABLE_SIZE 1000003
#define TABLE_SIZE 104729
#define TRANS_NULL 64000

#define FLAG_EXACT 0
#define FLAG_ALPHA 1
#define FLAG_BETA 2

#include <vector>
#include "board.h"

namespace checkers {
	class Board;

	struct Position
	{
		int depth;
		int value;
		int flag;
		Board board;
	};

	class TranspositionTable
	{
		public:
			TranspositionTable();
			~TranspositionTable();
			void update(Board& board, int depth, int value, int flag);
			int get(Board& board, int depth, int alpha, int beta);

			int size;
		private:
			inline unsigned int hash(Board& board);

			unsigned int bitstrings[160];
			Position* table;
	};
}

#endif // TRANPOSITION_H
