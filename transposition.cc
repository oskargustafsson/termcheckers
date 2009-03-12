#include "transposition.h"
#include "evaluation.h"
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
		null_position.depth = -1;
		null_position.value = TRANS_NULL;
		null_position.movecount = 0;

		new_values = 0;
		no_updates = 0;
		collisions = 0;
		total_get = 0;
		used_values = 0;
		size = 0;

		for(int i=0; i<161; i++)
		{
			zobristvalues[i] = std::rand();
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

	void TranspositionTable::update(Board& board, int depth, int value, int flag, int movecount)
	{
		if(value == ALPHA_WIN || value == -ALPHA_WIN || value == ALPHA_DRAW)
			return;
		int index = hash(board);
		Position last_pos = table[index];

		if(last_pos.value == TRANS_NULL)
			size++;
		if(last_pos.value == TRANS_NULL || movecount > last_pos.movecount+20 || depth > last_pos.depth)
		{
			new_values++;

			Position new_pos;
			new_pos.depth = depth;
			new_pos.value = value;
			new_pos.board = board;
			new_pos.flag = flag;
			new_pos.movecount = movecount;
			table[index] = new_pos;
		}
		else
			no_updates++;
	}

	int TranspositionTable::get(Board& board, int depth, int alpha, int beta)
	{
		Position p = table[hash(board)];
		total_get++;
		if(p.depth < depth || !(board == p.board))
		{
			if(!(board == p.board))
				collisions++;
			return TRANS_NULL;
		}
		if(p.flag == FLAG_EXACT)
		{
			used_values++;
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
		//unsigned int hash = 2166136261; // FNV hash
		//unsigned int g; // ELF hash

		int pos = 0;
		for(int i = 0; i < 32; i++) {
			pos = 0;
			pos += ((board.black & (~board.kings) & bit) != 0);
			pos += ((board.white & (~board.kings) & bit) != 0) * 32;
			pos += ((board.black & board.kings & bit) != 0) * 64;
			pos += ((board.white & board.kings & bit) != 0) * 96;
			pos += (pos == 0) * 128;

			hash ^= zobristvalues[i+pos];
			// trying different hash-functions
			//hash = (hash<<4) ^ (hash>>28) ^ zobristvalues[i+pos];
			//hash ^= (hash<<5) + (hash >> 2) + zobristvalues[i+pos];
			//hash = (hash * 16777619) ^ zobristvalues[i+pos]; // FNV
			/**ELF
			hash = (hash << 4) + zobristvalues[i+pos];
			g = hash & 0xF0000000L;
			if(g != 0)
				hash ^= g>>24;
			hash &= ~g;
			***ELF*/
			// couldnt see much of a difference, i think
			// which random numbers to use is more significant

			bit = bit<<1;
		}
		hash ^= zobristvalues[160] * (board.player == BLACK);

		return hash % TABLE_SIZE;
	}
}
