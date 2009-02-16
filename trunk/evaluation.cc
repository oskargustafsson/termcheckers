#include <iostream>
#include "evaluation.h"
#include "board.h"

// 		BLACK is +		WHITE is -
using namespace std;

int evaluate(Board& board) {
	int score = 0;

	///////////////////////////////////////////
	//	MATERIAL VALUE
	///////////////////////////////////////////

	// men (incl. kings)	(x128)
	score += board.countBits(board.black)<<7;
	score -= board.countBits(board.white)<<7;

	// kings							(x64)
	score += board.countBits(board.black & board.kings)<<6;
	score -= board.countBits(board.white & board.kings)<<6;


	///////////////////////////////////////////
	//	POSITIONAL VALUE
	///////////////////////////////////////////

	// number of move(locations)s			(x16)
	score += board.countBits(board.getMoves(board.black))<<4;
	score -= board.countBits(board.getMoves(board.white))<<4;

	// number of promting move(location)s								(x32)
	score += board.countBits(board.getMoves(board.black & (~board.kings) & 0x0F000000))<<5;
	score -= board.countBits(board.getMoves(board.white & (~board.kings) & 0x000000F0))<<5;

	// back rank							(x8)
	score += (board.countBits(board.black & 0x0000000F)<<3);
	score -= (board.countBits(board.white & 0xF0000000)<<3);
	
	// pieces along sides					(x4)
	score += (board.countBits(board.black & 0x00000000)<<2);
	score -= (board.countBits(board.white & 0x00000000)<<2);

	// dog hole (bad)													(x32)
	score -= (((board.white & 0x1<<31) != 0 ) && ((board.black & 0x1<<27) != 0 ))<<5 ;
	score += (((board.white & 0x1<<4) != 0 ) && ((board.black & 0x1) != 0 ))<<5;
	
	// current player (why?)	(x4)
	score += (board.player == BLACK)<<2;
	score -= (board.player == WHITE)<<2;
	
	return score;
}
