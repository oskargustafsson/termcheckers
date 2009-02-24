#include <iostream>
#include "evaluation.h"
#include "board.h"
#include "functions.h"

//              BLACK is +              WHITE is -
using namespace std;

int evaluate(Board& board) {
        int score = 0;

        int mvps = board.getMovePieces();
        int jmps = board.getJumpPieces();

        ///////////////////////////////////////////
        //      MATERIAL VALUE
        ///////////////////////////////////////////

        // men (incl. kings)    (x128)
        score += countBits(board.black)<<7;
        score -= countBits(board.white)<<7;

        // kings                                                        (x64)
        score += countBits(board.black & board.kings)<<6;
        score -= countBits(board.white & board.kings)<<6;


        ///////////////////////////////////////////
        //      POSITIONAL VALUE
        ///////////////////////////////////////////

        // number of move(locations)s                   (x16)
        score += countBits(board.getMoves(board.black))<<4;
        score -= countBits(board.getMoves(board.white))<<4;

        // number of promting move(location)s                                                           (x32)
        score += countBits(board.getMoves(board.black & (~board.kings) & 0x0F000000))<<5;
        score -= countBits(board.getMoves(board.white & (~board.kings) & 0x000000F0))<<5;

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
        score += (countBits(board.white) - jmps - mvps) * 30;
        score -= (countBits(board.black) - jmps - mvps) * 30;

        // make it good to win
        bool tmp = (jmps + mvps == 0);
        score -= (board.player == BLACK && tmp) * 10000;
        score += (board.player == WHITE && tmp) * 10000;

        return score;
}
