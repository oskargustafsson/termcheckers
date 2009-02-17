#include <cstdio>
#include <cmath>
#include <vector>
#include <iostream>
#include "search.h"
#include "evaluation.h"
#include "board.h"
#include "game.h"
#include "gui.h"

namespace checkers {

	Search::Search(Game* g) : game(g) {}
	Search::~Search() {}


	int Search::search() {
		int value = 0;

		if(singleJump(game->board)) {
			game->makeMove(movement);
			return value;
		}
		maxdepth = 15;
		value = alphabeta(game->board, maxdepth, -32000, 32000);
		reverse(movement);
		game->makeMove(movement);

		return value;
	}

	int Search::alphabeta(Board& board, int depth, int alpha, int beta) {
		unsigned int pieces = 0x0u;
		unsigned int moves = 0x0u;
		int tmp = 0;
		bool betacutoff = false;
		unsigned int from = 0x0u;
		unsigned int to = 0x0u;
		bool capture = false;

		// check if there is capture moves
		// and check which pieces can move
		if((pieces = board.getJumpPieces()) != 0x0u)
			capture=true;
		else
			pieces = board.getMovePieces();

		// Check if its the end node
		// if there is capture moves try one depth more
		if(((depth < 1) && !capture) || board.endOfGame()) {
			return board.player == BLACK ? evaluate(board) : -evaluate(board);
		}

		// For each move
		while(pieces != 0) {
			if(moves == 0) {
				from = (pieces & (pieces-1)) ^ pieces;
				if(capture) {
					moves = board.getCaptureMoves(from);
				} else {
					moves = board.getMoves(from);
				}
			}

			to = (moves & (moves-1)) ^ moves;
			moves &= moves-1;

			Board nextboard = board;
			nextboard.move(from, to);

			if(capture) {
				tmp = captureAlphaBeta(nextboard, depth, alpha, beta, to);
			} else {
				nextboard.changePlayer();
				nextboard.updateKings();
				tmp = -alphabeta(nextboard, depth-1, -beta, -alpha);
			}
			if(tmp > alpha) {
				alpha = tmp;
				if(depth == maxdepth) { // best movement so far, at the first depth
					best_movement.clear();
					for(unsigned int i=0; i < capture_movement.size(); i++) {
						best_movement.push_back(capture_movement[i]);
					}
					if(board.countBits(board.getCaptureMoves(from)) != 1)
						best_movement.push_back(to);
					best_movement.push_back(from);
				}
			}
			if(beta <= alpha) {
				betacutoff = true;
				break;
			}
			if(moves == 0)
				pieces &= pieces-1;
		}
		if(depth == maxdepth) {
			// The root node, make the best move
			movement.clear();
			movement = best_movement;
		}
		return alpha;
	}

	int Search::captureAlphaBeta(Board& board, int depth, int alpha, int beta, unsigned int from) {
		unsigned int moves = board.getCaptureMoves(from);
		int nrOfMoves = board.countBits(moves);
		unsigned int moveTo;
		unsigned int to = 0x0u;
		int tmp;

		if(moves == 0) {
			board.changePlayer();
			board.updateKings();
			tmp = -alphabeta(board, depth-1, -beta, -alpha);
			if(tmp > alpha) {
				alpha = tmp;
				if(depth == maxdepth) {
					capture_movement.clear();
					capture_movement.push_back(from);
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

		if(nrOfMoves > 1 && depth == maxdepth && capture_movement.back() != moveTo) {
			capture_movement.push_back(moveTo);
		}

		return alpha;
	}

	bool Search::singleJump(Board board) {
		unsigned int to;
		int bits;
		unsigned int from = board.getJumpPieces();

		if(board.countBits(from) != 1)
			return false;

		movement.clear();
		movement.push_back(from);

		to = board.getCaptureMoves(from);
		bits = board.countBits(to);
		while(bits == 1) {
			board.move(from, to);
			from = to;
			to = board.getCaptureMoves(from);
			bits = board.countBits(to);
		}
		movement.push_back(from);

		return bits == 0;
	}

	void Search::reverse(std::vector<unsigned int>& list) {
		std::vector<unsigned int> tmp_list = list;

		list.clear();
		while(!tmp_list.empty()) {
			list.push_back(tmp_list.back());
			tmp_list.pop_back();
		}
	}
}
