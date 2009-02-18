#ifndef SEARCHTREE_H
#define SEARCHTREE_H

#include "board.h"
#include "timer.h"
#include <vector>

// DEFINE WHAT OPTIMIZATIONS TO USE:
#define KILLER_MOVES

namespace checkers {
	class Game;

	class Search {
		public:
			Search(Game* g);
			~Search();

			int search();
		private:
			int alphabeta(Board& b, int depth, int alpha, int beta);
			int captureAlphaBeta(Board& b, int depth, int alpha, int beta, unsigned int from);
			bool singleJump(Board b);
			void reverse(std::vector<unsigned int>& list);
			void newBestMove(Board& board, unsigned int from, unsigned int to);

			Game* game;
			int maxdepth;
			double nrOfNodes;
			std::vector<unsigned int>* movement;
			std::vector<unsigned int>* capture_movement;
			unsigned int recursiveTo;

			timer::Timer* timer;
#ifdef KILLER_MOVES
			unsigned int killer[100];
#endif

			struct shortMove {
				unsigned int from;
				unsigned int to;
			};
	};
}

#endif
