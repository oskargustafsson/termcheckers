#include <cstdio>
#include <iostream>
#include "search.h"
#include "evaluation.h"
#include "board.h"
#include "game.h"
#include "gui.h"

namespace checkers {

	Search::Search(Game* g) : game(g) {}
	Search::~Search() {}

	int Search::alphabeta(Board& board, int depth) {
			 maxdepth = depth;
			 return alphabeta(board, depth, -32000, 32000);
	}

	int Search::alphabeta(Board& board, int depth, int alpha, int beta) {
			 unsigned int men;
			 unsigned int moves;
			 int tmp;
			 bool betacutoff = false;
			 unsigned int moveFrom = 0x0u;
			 unsigned int from = 0x0u;
			 unsigned int to = 0x0u;
			 unsigned int moveTo = 0x0u;
			 int capture = 0;

			 board.player == WHITE ? men = board.white : men = board.black;

			 /*
			* test optimiering:
			* ta bort några som garanterat inte kan hoppa
			* inte säkert att det är bättre. och i vilket fall som helst så blir det nog ingen större skillnad...
			*/
			 men = (((men<<9) & ~(board.black & board.white))>>9) |
				(((men<<7) & ~(board.black & board.white))>>7) |
				(((men>>9) & ~(board.black & board.white))<<9) |
				(((men>>7) & ~(board.black & board.white))<<7);

			 // Count number of capture moves:
			 // TODO: try and do this without loop!
			 while(men != 0) {
				from = (men & (men-1)) ^ men;
				men &= men-1;
				if(board.getCaptureMoves(from) != 0) {
					capture++;
				}
			 }

			 // Check if its the end node
			 // if there is capture moves try one depth more
			 if(board.endOfGame() || ((depth < 1) && (capture == 0))) {
				return board.player == BLACK ? evaluate(board) : -evaluate(board);
			 }

			 // For each move
			 board.player == WHITE ? men = board.white : men = board.black;
			 while(men != 0) {
				from = (men & (men-1)) ^ men;
				men &= men-1;

				if(capture == 0) {
					moves = board.getMoves(from);
				} else {
					moves = board.getCaptureMoves(from);

					// break if there is only one capture move to do
					// maybe move this outside alphabeta?
					if(moves != 0 && capture == 1 && depth == maxdepth && oneCapture(board, from, moves)) {
							 moveFrom = from;
							 moveTo = recursiveTo;
							 break;
					}
				}

				while(moves != 0) {
					to = (moves & (moves-1)) ^ moves;
					moves &= moves-1;

					Board nextboard = board;
					nextboard.move(from, to);

					if(capture != 0) {
							 tmp = captureAlphaBeta(nextboard, depth, alpha, beta, to);
							 to = recursiveTo;
					} else {
							 nextboard.changePlayer();
							 nextboard.updateKings();
							 tmp = -alphabeta(nextboard, depth-1, -beta, -alpha);
					}
					if(tmp > alpha) {
							 alpha = tmp;
							 moveFrom = from;
							 moveTo = to;
					}
					if(beta <= alpha) {
							 betacutoff = true;
							 break;
					}
				}
				if(betacutoff) {
					break;
				}
			 }
			 if(depth == maxdepth) {
				// The root node, make the best move
				std::vector<int> movement;
				movement.push_back(moveFrom);
				movement.push_back(moveTo);
				game->makeMove(movement, 2);
			 }
			 return alpha;
	}

	int Search::captureAlphaBeta(Board& board, int depth, int alpha, int beta, unsigned int from) {
			 unsigned int moves = board.getCaptureMoves(from);
			 unsigned int to = 0x0u;
			 int tmp;

			 if(moves == 0) {
				board.changePlayer();
				board.updateKings();
				tmp = -alphabeta(board, depth-1, -beta, -alpha);
				if(tmp > alpha) {
					alpha = tmp;
					if(depth == maxdepth) {
							 recursiveTo = from;
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
				}
			 }

			 return alpha;
	}

	bool Search::oneCapture(Board board, unsigned int from, unsigned int moves) {
			 int bits = board.countBits(moves);

			 while(bits == 1) {
				board.move(from, moves);
				from = moves;
				moves = board.getCaptureMoves(from);
				bits = board.countBits(moves);
			 }
			 recursiveTo = from;

			 return bits == 0;
	}
}
