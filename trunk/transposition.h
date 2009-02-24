#ifndef TRANSTABLE_H
#define TRANSTABLE_H

#include "board.h"
#include <vector>

namespace checkers {

	class TransTable {
		public:
			TransTable();
			~TransTable();
			void add(int value);
			void remove(Board& board);
		private:
			inline int hash(Board& board);

			unsigned int bitstrings[128];
			std::vector<int>* table[1000];
			std::vector<int>** listPtr;
			int size;
	};
}

#endif // TRANSTABLE_H
