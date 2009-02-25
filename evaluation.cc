#include <iostream>
#include "evaluation.h"
#include "board.h"

#define countBits(pieces) (__builtin_popcount(pieces))

//              BLACK is +              WHITE is -
using namespace std;

int evaluate(Board& board, int depth) {
		  int score = 0;

		  int black_mvps, black_jmps, white_mvps, white_jmps;

		  int black_pieces = countBits(board.black);
		  int white_pieces = countBits(board.white);

		  if(board.player == BLACK) {
					 black_mvps = board.getMovePieces();
					 black_jmps = board.getJumpPieces();
					 board.changePlayer();
					 white_mvps = board.getMovePieces();
					 white_jmps = board.getJumpPieces();
		  } 
		  else {
					 white_mvps = board.getMovePieces();
					 white_jmps = board.getJumpPieces();
					 board.changePlayer();
					 black_mvps = board.getMovePieces();
					 black_jmps = board.getJumpPieces();
		  }
		  board.changePlayer();

		  // make it good to win
		  score -= (board.player == BLACK && (black_jmps | black_mvps) == 0) * (32000 - depth);
		  score += (board.player == WHITE && (white_jmps | white_mvps) == 0) * (32000 - depth);

		  // this is a win or loss
		  if(score != 0) {
					 return score;
		  }

			// ENDGAME
			/*
		  if(black_pieces + white_pieces <= 8)
		  {
		  		if(black_pieces > white_pieces)
						  score += countBits(board.box(board.kings & board.black) & board.white)<<4;
				if(white_pieces < black_pieces)
						  score -= countBits(board.box(board.kings & board.white) & board.black)<<4;
		  }
		  */

		  ///////////////////////////////////////////
		  //      MATERIAL VALUE
		  ///////////////////////////////////////////

		  // men (incl. kings)    (x128)
		  score += black_pieces<<7;
		  score -= white_pieces<<7;

		  // kings                                                        (x64)
		  score += countBits(board.black & board.kings)<<6;
		  score -= countBits(board.white & board.kings)<<6;


		  ///////////////////////////////////////////
		  //      POSITIONAL VALUE
		  ///////////////////////////////////////////

		  // number of move(locations)s                   (x16)
		  score += countBits(board.getMoves(board.black))<<4;
		  score -= countBits(board.getMoves(board.white))<<4;

		  // number of promting move(location)s                                                           (x16))
		  score += countBits(board.getMoves(board.black & (~board.kings) & 0x0F000000))<<4;
		  score -= countBits(board.getMoves(board.white & (~board.kings) & 0x000000F0))<<4;

		  // back rank                                                    (x8)
		  score += (countBits(board.black & 0x0000000F)<<3);
		  score -= (countBits(board.white & 0xF0000000)<<3);

		  // dog hole (bad)                                                                                                       (x32)
		  score -= (((board.white & 0x1<<31) != 0 ) && ((board.black & 0x1<<27) != 0 ))<<5 ;
		  score += (((board.white & 0x1<<4) != 0 ) && ((board.black & 0x1) != 0 ))<<5;

		  // current player (why?)        (x4)
		  score += (board.player == BLACK)<<2;
		  score -= (board.player == WHITE)<<2;

		  // Make it good to imobilize opponents peices
		  score += (countBits(board.white & (white_jmps | white_mvps))) * 30;
		  score -= (countBits(board.black & (black_jmps | black_mvps))) * 30;

		  // middle 2x2 square
		  score += countBits(0x00666600 & board.black) * 8;
		  score -= countBits(0x00666600 & board.white) * 8;

		  return score;
}
