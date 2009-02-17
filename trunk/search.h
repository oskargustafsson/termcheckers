#ifndef SEARCHTREE_H
#define SEARCHTREE_H

#include "board.h"
#include <vector>

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

			Game* game;
			int maxdepth;
			std::vector<unsigned int> movement;
			std::vector<unsigned int> capture_movement;
			std::vector<unsigned int> best_movement;
			unsigned int recursiveTo;

			unsigned int* killer;

			struct shortMove {
				unsigned int from;
				unsigned int to;
			};
	};
}

#endif
