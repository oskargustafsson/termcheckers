#include <iostream>
#include <vector>
#include <algorithm>
#include "search.h"
#include "evaluation.h"
#include "board.h"
#include "game.h"
#include "timer.h"
#include "functions.h"

namespace checkers {

        /*
         * CONSTRUCTOR
         */
        Search::Search(Game* g) : game(g)
        {
                movement = new std::vector<unsigned int>;
                capture_movement = new std::vector<unsigned int>;
                timer = new timer::Timer();
#ifdef KILLER_MOVES
                for(int i=0; i<100; ++i) {
                        killer[i] = 0x0u;
                }
#endif // KILLER_MOVES
        }

        /*
         * DESTRUCTOR
         */
        Search::~Search()
        {
                delete movement;
                delete capture_movement;
                delete timer;
        }

        /*
         * Only public function
         * returns the value of the alphabeta algorthm
         * and make the move in game
         */
        int Search::search()
        {
                int value = 0;
                nrOfNodes = 0;

                timer->startTimer();

                if(singleJump(game->board)) {
                        game->makeMove(*movement);
                        return value;
                }
                int i=1;
                while(timer->getTime() < 1000000) {
                        maxdepth = i;
                        value = alphabeta(game->board, 0, -32000, 32000);
                        ++i;
                }

                std::reverse(movement->begin(), movement->end());
                game->makeMove(*movement);

                time = timer->stopTimer();
                return value;
        }

        /*
         * ALPHABETA
         */
        int Search::alphabeta(Board& board, int depth, int alpha, int beta)
        {
                unsigned int pieces = 0x0u;
                unsigned int moves = 0x0u;
                unsigned int from = 0x0u;
                unsigned int to = 0x0u;
                bool capture = false;
                int tmp = 0;

                nrOfNodes++;

                // check if there is capture moves
                // and check which pieces can move
                if((pieces = board.getJumpPieces()) != 0x0u)
                        capture=true;
                else
                        pieces = board.getMovePieces();

                // Check if its the end node
                // if there is capture moves try one depth more
                if(((depth >= maxdepth) && !capture) || pieces == 0)
                {
                        return board.player == BLACK ? evaluate(board) : -evaluate(board);
                }

#ifdef KILLER_MOVES
                if(depth >= 0)
                        from = pieces & killer[depth];
                if((from != 0) && (countBits(from) == 1))
                {
                        if(capture)
                                moves = board.getCaptureMoves(from);
                        else
                                moves = board.getMoves(from);
                        to = moves & killer[depth];
                        if(to != 0x0u) // KILLER IS LEGAL
                        {
                                moves = to;
                                /*********************************
                                * A small problem is that it might
                                * check the killer node again.
                                * I cant remove "from" from pieces
                                * without missing the other moves
                                * the "from" piece can do, thus
                                * pieces &= ~from is not an option
                                *********************************/
                        }
                        else
                        {
                                moves = 0x0u;
                        }
                }
#endif //KILLER_MOVES

                // For each possible move
                while(pieces != 0x0u)
                {
                        Board nextboard = board;

                        if(moves == 0x0u)
                        {
                                from = (pieces & (pieces-1)) ^ pieces;
                                if(capture)
                                        moves = board.getCaptureMoves(from);
                                else
                                        moves = board.getMoves(from);
                        }
                        to = (moves & (moves-1)) ^ moves;
                        moves &= moves-1;

                        nextboard.move(from, to);

                        if(capture) {
                                tmp = captureAlphaBeta(nextboard, depth, alpha, beta, to);
                        } else {
                                nextboard.changePlayer();
                                nextboard.updateKings();
                                tmp = -alphabeta(nextboard, depth+1, -beta, -alpha);
                        }
                        if(tmp > alpha) {
                                alpha = tmp;
                                if(depth == 0) {
                                        newBestMove(board, from, to);
                                }
                        }
                        if(beta <= alpha) {
#ifdef KILLER_MOVES
                                if(depth >= 0)
                                        killer[depth] = from | to;
#endif // KILLER_MOVES
                                break;
                        }
                        if(moves == 0)
                                pieces &= pieces-1;
                }

                return alpha;
        }


        /*
         * Help function for alphabeta
         * used when one move does multiple jumps
         */
        int Search::captureAlphaBeta(Board& board, int depth, int alpha, int beta, unsigned int from) {
                unsigned int moves = board.getCaptureMoves(from);
                int nrOfMoves = countBits(moves);
                unsigned int moveTo;
                unsigned int to = 0x0u;
                int tmp;

                if(moves == 0) {
                        Board nextboard = board;
                        nextboard.changePlayer();
                        nextboard.updateKings();
                        tmp = -alphabeta(nextboard, depth+1, -beta, -alpha);
                        if(tmp > alpha) {
                                alpha = tmp;
                                if(depth == 0) {
                                        capture_movement->clear();
                                        capture_movement->push_back(from);
                                }
                        }
                }

                while(moves != 0) {
                        to = (moves & (moves-1)) ^ moves;
                        moves &= moves-1;

                        Board nextboard = board;
                        nextboard.move(from, to);

                        tmp = captureAlphaBeta(nextboard, depth, alpha, beta, to);
                        if(tmp > alpha) {
                                alpha = tmp;
                                moveTo = to;
                        }
                }

                if((nrOfMoves > 1) && (depth == 0) && (capture_movement->back() != moveTo)) {
                        capture_movement->push_back(moveTo);
                }

                return alpha;
        }

        /*
         * Check if there is just one option this move
         */
        bool Search::singleJump(Board board)
        {
                unsigned int to;
                int bits;
                unsigned int from = board.getJumpPieces();

                if(countBits(from) != 1)
                        return false;

                movement->clear();
                movement->push_back(from);

                to = board.getCaptureMoves(from);
                bits = countBits(to);
                while(bits == 1) {
                        board.move(from, to);
                        from = to;
                        to = board.getCaptureMoves(from);
                        bits = countBits(to);
                }
                movement->push_back(from);

                return bits == 0;
        }

        /**
         * Best move so far!
         */
        void Search::newBestMove(Board& board, unsigned int from, unsigned int to)
        {
                movement->clear();
                for(unsigned int i=0; i < capture_movement->size(); i++)
                {
                        movement->push_back((*capture_movement)[i]);
                }
                if(countBits(board.getCaptureMoves(from)) != 1)
                {
                        movement->push_back(to);
                }
                movement->push_back(from);
        }
}
