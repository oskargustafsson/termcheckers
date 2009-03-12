#ifndef MOVE_H
#define MOVE_H

#include <vector>
#include <iostream>

namespace checkers
{
	class Board;

	class Move
	{
		public:
			Move();
			Move(const std::vector<unsigned int>&);
			Move(const std::string&);
			~Move();

			void add(unsigned int);
			void clear();
			int validate(Board&);
			unsigned int first() const;

			bool makeNext(Board&);

			const Move& operator=(const std::vector<unsigned int>&);
			friend std::ostream& operator<<(std::ostream&, const Move&);

			bool begun;
			int time;
		private:
			std::vector<unsigned int> move;
			std::vector<unsigned int> full;
			std::vector<unsigned int>::iterator iter;
	};

}

#endif // MOVE_H
