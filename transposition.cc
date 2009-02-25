#include "transposition.h"
#include "board.h"
#include <cstdlib>
#include <ctime>
#include <iostream>

namespace checkers {

	TranspositionTable::TranspositionTable()
	{
		table = new Position[TABLE_SIZE];
		std::srand(std::time(NULL));
		Position null_position;
		null_position.depth = 0;
		null_position.value = TRANS_NULL;

		for(int i=0; i<161; i++)
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

	void TranspositionTable::update(Board& board, int depth, int value, int flag)
	{
		int index = hash(board);
		Position last_pos = table[index];
		if(depth > last_pos.depth)
		{
			Position new_pos;
			new_pos.depth = depth;
			new_pos.value = value;
			new_pos.board = board;
			new_pos.flag = flag;
			table[index] = new_pos;
		}
	}

	int TranspositionTable::get(Board& board, int depth, int alpha, int beta)
	{
		Position p = table[hash(board)];
		if(p.depth < depth || !(board == p.board))
		{
			return TRANS_NULL;
		}
		if(p.flag == FLAG_EXACT)
		{
			return p.value;
		}
		if((p.flag == FLAG_ALPHA) && (p.value <= alpha))
		{
			return alpha;
		}
		if((p.flag == FLAG_BETA) && (p.value >= beta))
		{
			return beta;
		}
		return TRANS_NULL;
	}

	inline unsigned int TranspositionTable::hash(Board& board)
	{
		unsigned int hash = 0;
		unsigned int bit = 1;
/*		
		for(int i = 0; i < 32; i++)
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
*/

		int pos = 0;
		for(int i = 0; i < 32; i++) {
			pos = 0;
			pos += ((board.white & (~board.kings) & bit) != 0) * 32;
			pos += ((board.black & board.kings & bit) != 0) * 64;
			pos += ((board.white & board.kings & bit) != 0) * 96;
			pos += (bit == 0) * 128;
			hash ^= bitstrings[i+pos];
			bit = bit<<1;
		}
		if(board.player == BLACK)
			hash ^= bitstrings[160];

		return hash % TABLE_SIZE;
	}
}
