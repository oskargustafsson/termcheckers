#ifndef SEARCH_H
#define SEARCH_H

#include "board.h"
#include "timer.h"
#include <vector>

#define MAX_TIME 1000000

// DEFINE WHAT OPTIMIZATIONS TO USE:
#define SCOUT
#define HISTORY_HEURISTIC
#define ITERATIVE_DEEPENING

namespace checkers {

	struct SearchResult {
		int nodes;
		int depth;
		int time;
		int value;
		std::vector<unsigned int> move;
	};

	class Search {
		public:
			Search();
			~Search();

			SearchResult search(Board board);
			void sortTest();

			int time;
			int maxdepth;
			int nrOfNodes;
		private:
			int alphabeta(Board& b, int depth, int alpha, int beta);
			int captureAlphaBeta(Board& b, int depth, int alpha, int beta, unsigned int from);
			bool singleJump(Board b);
			void reverse(std::vector<unsigned int>& list);
			void newBestMove(Board& board, unsigned int from, unsigned int to);
			void sortMovesHeap(unsigned int movelist[], int movevalues[], unsigned int movecount);
			inline void siftDown(unsigned int movelist[], int movevalues[], int start, int end);
			inline void swap(unsigned int movelist[], int movevalues[], int a, int b);

			inline void insertMove(unsigned int movelist[], int movevalues[], unsigned int from, unsigned int to, int newValue, unsigned int& movecount);

			std::vector<unsigned int>* movement;
			std::vector<unsigned int>* capture_movement;

			timer::Timer* timer;

#ifdef HISTORY_HEURISTIC
			int history[32][32];
#endif // HISTORY_HEURISTIC

	};
}

#endif // SEARCH_H
