#include <iostream>
#include "evaluation.h"
#include "board.h"

// 		BLACK is +		WHITE is -
using namespace std;

int evaluate(board& b) {
	int score = 0;

	///////////////////////////////////////////
	//	MATERIAL VALUE
	///////////////////////////////////////////

	// men (incl. kings)	(x128)
	score += countBits(b.black)<<7;
	score -= countBits(b.white)<<7;

	// kings							(x64)
	score += countBits(b.black & b.kings)<<6;
	score -= countBits(b.white & b.kings)<<6;


	///////////////////////////////////////////
	//	POSITIONAL VALUE
	///////////////////////////////////////////

	// number of move(locations)s			(x16)
	score += countBits(getMoves(b, b.black))<<4;
	score -= countBits(getMoves(b, b.white))<<4;

	// number of promting move(location)s								(x32)
	score += countBits(getMoves(b, (b.black & (~b.kings) & 0x0F000000)))<<5;
	score -= countBits(getMoves(b, (b.white & (~b.kings) & 0x000000F0)))<<5;

	// back rank							(x8)
	score += (countBits(b.black & 0x0000000F)<<3);
	score -= (countBits(b.white & 0xF0000000)<<3);
	
	// pieces along sides					(x4)
	score += (countBits(b.black & 0x00000000)<<2);
	score -= (countBits(b.white & 0x00000000)<<2);

	// dog hole (bad)													(x32)
	score -= (((b.white & 0x1<<31) != 0 ) && ((b.black & 0x1<<27) != 0 ))<<5 ;
	score += (((b.white & 0x1<<4) != 0 ) && ((b.black & 0x1) != 0 ))<<5;
	
	// current player (why?)	(x4)
	score += (b.player == BLACK)<<2;
	score -= (b.player == WHITE)<<2;
	
	return score;
}
