#include "move.h"
#include "board.h"
#include <vector>
#include <iostream>
#include <cmath>
#include <string>
#include <cstdlib>

namespace checkers
{
	Move::Move() : begun(false),time(0) {}

	Move::Move(const std::vector<unsigned int>& m) : begun(false),time(0),move(m){}

	Move::Move(const std::string& str) : begun(false), time(0)
	{
		std::string::const_iterator It =  str.begin();
		int i=0;
		std::string tmpstr;

		while( It != str.end()) {
			if(*It == '-') {
				add(static_cast<unsigned int>(pow(2.0, atof(tmpstr.c_str())-1)));
				tmpstr = "";
				i++;
				It++;
				continue;
			}
			tmpstr += *It;
			It++;
		}
		add(static_cast<unsigned int>(pow(2.0, atof(tmpstr.c_str())-1)));
	}

	Move::~Move(){}

	int Move::validate(Board& board)
	{
		full = move;
		return board.validateMove(full);
	}

	void Move::add(unsigned int to)
	{
		move.push_back(to);
	}

	void Move::clear()
	{
		move.clear();
	}

	unsigned int Move::first() const
	{
		return move.front();
	}

	bool Move::makeNext(Board& board)
	{
		unsigned int tmp;
		if(!begun) {
			iter = full.begin();
			begun = true;
		}
		if(iter == full.end())
			return false;
		tmp = *iter;
		iter++;
		if(iter == full.end())
			return false;
		board.move(tmp, *iter);
		return iter != full.end();
	}

	const Move& Move::operator=(const std::vector<unsigned int>& m)
	{
		move = m;
		full.clear();
		begun = false;
		return *this;
	}

	std::ostream& operator<<(std::ostream& os, const Move& move)
	{
		for(size_t i = 0; i<move.move.size(); i++)
		{
			if(i > 0) os << "-";
			os << log2(move.move[i]) + 1;
		}
		return os;
	}
}
