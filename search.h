#ifndef SEARCHTREE_H
#define SEARCHTREE_H

#include "board.h"
#include "timer.h"
#include <vector>

#define MAX_TIME 100000

// DEFINE WHAT OPTIMIZATIONS TO USE:
//#define KILLER_MOVES
//#define KILLER_SIZE 100
//#define SCOUT
#define HISTORY_HEURISTIC

namespace checkers {
	class Game;

	class Search {
		public:
			Search(Game* g);
			~Search();

			int search();
			void sortTest();

			int time;
			int maxdepth;
			double nrOfNodes;
		private:
			int alphabeta(Board& b, int depth, int alpha, int beta);
			int captureAlphaBeta(Board& b, int depth, int alpha, int beta, unsigned int from);
			bool singleJump(Board b);
			void reverse(std::vector<unsigned int>& list);
			void newBestMove(Board& board, unsigned int from, unsigned int to);
			void sortMoves(unsigned int movelist[], int movevalues[], unsigned int movecount);
			inline void siftDown(unsigned int movelist[], int movevalues[], int start, int end);
			inline void swap(unsigned int movelist[], int movevalues[], int a, int b);
			struct shortMove {
				unsigned int from;
				unsigned int to;
			};

			Game* game;
			std::vector<unsigned int>* movement;
			std::vector<unsigned int>* capture_movement;

			timer::Timer* timer;
#ifdef KILLER_MOVES
			unsigned int killer[KILLER_SIZE];
#endif // KILLER_MOVES

#ifdef HISTORY_HEURISTIC
			int history[32][32];
#endif // HISTORY_HEURISTIC

	};
}

#endif