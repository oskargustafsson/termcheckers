#ifndef TRANSPOSITION_H
#define TRANSPOSITION_H

#define TABLE_SIZE 514229
#define TRANS_DEPTH 8
#define TRANS_NULL 64000

#include <vector>
#include "board.h"

namespace checkers {
	class Board;

	struct Position
	{
		int depth;
		int value;
		Board board;
	};

	class TranspositionTable
	{
		public:
			TranspositionTable();
			~TranspositionTable();
			void add(Board& board, int depth, int value);
			int get(Board& board, int depth);

			int size;
		private:
			inline unsigned int hash(Board& board);

			unsigned int bitstrings[160];
			Position* table;
	};
}

#endif // TRANPOSITION_H
