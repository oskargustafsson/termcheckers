#include "transposition.h"
#include "board.h"

namespace checkers {

	TransTable::TransTable() : size(0)
	{
		srand(time(NULL));
		for(int i=0; i<128; i++)
		{
			bitstrings[i] = rand();
		}
	}

	TransTable::~TransTable()
	{
		delete table;
	}

	void TransTable::add(Board& board, int value)
	{
		int index = hash(board);
		listPtr = &Table[index];
		listPtr->push_back(value);
		size++;
	}

	void TransTable::remove(Board& board)
	{
		int index = hash(board);
		listPtr = &Table[index];
	}

	inline int hash(Board& board)
	{
	}
}
