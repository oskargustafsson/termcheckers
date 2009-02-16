#ifndef SEARCHTREE_H
#define SEARCHTREE_H

#define DEPTH 10

#include "board.h"

namespace checkers {
	class Game;

	  class Search {
			 public:
				Search(Game* g);
				~Search();

				int alphabeta(Board& b, int depth);

			 private:
				int alphabeta(Board& b, int depth, int alpha, int beta);
				int captureAlphaBeta(Board& b, int depth, int alpha, int beta, unsigned int from);
				bool oneCapture(Board b, unsigned int from, unsigned int moves);

				Game* game;
				int maxdepth;
				unsigned int recursiveTo;
	  };
}

#endif
