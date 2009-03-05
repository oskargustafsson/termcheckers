#ifndef SEARCH_H
#define SEARCH_H

#include "board.h"
#include "timer.h"
#include <vector>

// DEFINE WHAT OPTIMIZATIONS TO USE:
#define SCOUT
#define HISTORY_HEURISTIC
#define TRANS_TABLE

namespace checkers {
	class TranspositionTable;
	struct SearchResult;
	class Game;

	class Player {
		public:
			Player(Game*);
			~Player();

			SearchResult search();
			int getTime();
		private:
			Game* game;
			int alphabeta(Board& b, int depth, int alpha, int beta);
			int captureAlphaBeta(Board& b, int depth, int alpha, int beta, unsigned int from);
			bool singleJump(Board b);
			void newBestMove(Board& board, unsigned int from, unsigned int to);
			inline void swap(unsigned int movelist[], int movevalues[], int a, int b);
			inline void insertMove(unsigned int movelist[], int movevalues[], unsigned int from, unsigned int to, int newValue, unsigned int& movecount);

			std::vector<unsigned int>* movement;
			std::vector<unsigned int>* capture_movement;

			timer::Timer* timer;

			int maxdepth;
			int nrOfNodes;
			int extendedDepth;
			bool finished_search;
			int time_check;
			int max_time;
#ifdef HISTORY_HEURISTIC
			int history[32][32];
#endif // HISTORY_HEURISTIC

#ifdef TRANS_TABLE
			TranspositionTable* trans_table;
#endif // TRANS_TABLE

	};
}

#endif // SEARCH_H
