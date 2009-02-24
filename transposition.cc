#include "transposition.h"
#include "board.h"
#include <cstdlib>
#include <iostream>

namespace checkers {

	TranspositionTable::TranspositionTable() : size(0)
	{
		table = new Position[TABLE_SIZE];
		std::srand(std::time(NULL));
		Position null_position;
		null_position.depth = 0;
		null_position.value = TRANS_NULL;

		for(int i=0; i<160; i++)
		{
			bitstrings[i] = std::rand();
		}
		for(int i=0; i<TABLE_SIZE; i++)
		{
			table[i] = null_position;
		}
	}

	TranspositionTable::~TranspositionTable()
	{
		delete[] table;
	}

	void TranspositionTable::add(Board& board, int depth, int value)
	{
		int index = hash(board);
		Position last_pos = table[index];
		if(depth > last_pos.depth)
		{
			Position new_pos;
			new_pos.depth = depth;
			new_pos.value = value;
			new_pos.board = board;
			table[index] = new_pos;
			if(last_pos.depth == 0)
			{
				size++;
			}
		}
	}

	int TranspositionTable::get(Board& board, int depth)
	{
		int index = hash(board);
		Position p = table[index];
		if(p.depth >= depth && board == p.board)
			return p.value;
		else
			return TRANS_NULL;
	}

	inline unsigned int TranspositionTable::hash(Board& board)
	{
		unsigned int hash = 0;
		unsigned int bit = 1;
		for(int i = 1; i < 32; i++)
		{
			if((board.black & bit) != 0)
				hash ^= bitstrings[i];
			else if((board.white & bit) != 0)
				hash ^= bitstrings[i+32];
			else if((board.black & board.kings & bit) != 0)
				hash ^= bitstrings[i+64];
			else if((board.white & board.kings & bit) != 0)
				hash ^= bitstrings[i+96];
			else
				hash ^= bitstrings[i+128];
			bit = bit<<1;
		}
		return hash % TABLE_SIZE;
	}
}
