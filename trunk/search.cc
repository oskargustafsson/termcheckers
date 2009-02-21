#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
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
		for(int i=0; i < KILLER_SIZE; i++)
			killer[i] = 0x0u;
#endif // KILLER_MOVES
#ifdef HISTORY_HEURISTIC
		for(int a=0; a<32; ++a) {
			for(int b=0; b<32; ++b) {
				history[a][b] = 0;
			}
		}
#endif // HISTORY_HEURISTIC

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
	 * The only public function
	 * returns the value of the alphabeta algorthm
	 * and make the move in game
	 */
	int Search::search()
	{
		int value = 0;
		nrOfNodes = 0;
		maxdepth = 0;

		timer->startTimer();

		if(singleJump(game->board)) {
			game->makeMove(*movement);
			time = timer->stopTimer();
			return value;
		}
		while(timer->getTime() < MAX_TIME) {
			maxdepth++;
			value = alphabeta(game->board, 0, -32000, 32000);
		}
		/**
		 * If we stop alphabeta before its finished,
		 * remember that movement might contain wrong moves.
		 * Use another vector for finished calculations
		 */

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
		unsigned int movelist[96];
#ifdef HISTORY_HEURISTIC
		int movevalues[48] = {0};
#endif // HISTORY_HEURISTIC
		unsigned int movecount = 0;
#ifdef SCOUT
		int testBeta = beta;
#endif // SCOUT

		nrOfNodes++;

		/*********************************
		 * Check if there is capture moves
		 * and which pieces can move.
		 *********************************/
		if((pieces = board.getJumpPieces()) != 0x0u)
			capture=true;
		else
			pieces = board.getMovePieces();

		/*******************************
		 * Check if its the end node.
		 * If there is capture moves,
		 * try one more ply.
		 *******************************/
		if(((depth >= maxdepth) && !capture) || pieces == 0)
		{
			return board.player == BLACK ? evaluate(board) : -evaluate(board);
		}

		/**********************
		 * GENERATE THE MOVES:
		 **********************/
		while(pieces != 0x0u || moves != 0x0u)
		{
			if(moves == 0x0u)
			{
				from = (pieces &(pieces-1)) ^ pieces;
				pieces &= pieces-1;
				if(capture)
					moves = board.getCaptureMoves(from);
				else
					moves = board.getMoves(from);
			}
			to = (moves & (moves-1)) ^ moves;
			moves &= moves-1;

			movelist[movecount<<1] = from;
			movelist[(movecount<<1)+1] = to;
#ifdef HISTORY_HEURISTIC
			movevalues[movecount] = history[bitToDec(from)][bitToDec(to)];
#endif // HISTORY_HEURISTIC
#ifdef KILLER_MOVES
			// Put killer move first
			if((killer[depth] & to) != 0 && (killer[depth] & from) != 0)
			{
				unsigned int tmp;
				tmp = movelist[0];
				movelist[0] = from;
				movelist[movecount] = tmp;
				tmp = movelist[1];
				movelist[1] = to;
				movelist[movecount+1] = tmp;
			}
#endif //KILLER_MOVES
			movecount++;

		}

#ifdef HISTORY_HEURISTIC
		sortMoves(movelist, movevalues, movecount);
#endif // HISTORY_HEURISTIC

		/*****************
		 * FOR EACH MOVE
		 *****************/
		for(unsigned int i=0; i<movecount; i++)
		{
			Board nextboard = board;

			from = movelist[i<<1];
			to = movelist[(i<<1)+1];

			nextboard.move(from, to);

			if(capture)
			{
				tmp = captureAlphaBeta(nextboard, depth, alpha, beta, to);
			}
			else
			{
				nextboard.changePlayer();
				nextboard.updateKings();
#ifdef SCOUT
				tmp = -alphabeta(nextboard, depth+1, -testBeta, -alpha);
				if(tmp >= testBeta && testBeta < beta)
				{
					tmp = -alphabeta(nextboard, depth+1, -beta, -alpha);
				}
#else
				tmp = -alphabeta(nextboard, depth+1, -beta, -alpha);
#endif // SCOUT
			}
			if(tmp > alpha)
			{
				alpha = tmp;
				if(depth == 0)
				{
					newBestMove(board, from, to);
				}
				if(alpha >= beta)
				{
#ifdef KILLER_MOVES
					killer[depth] = from | to;
#endif // KILLER_MOVES
#ifdef HISTORY_HEURISTIC
					// TODO Change increment value
					history[bitToDec(from)][bitToDec(to)]-=depth;
#endif // HISTORY_HEURISTIC
					break;
				}
#ifdef SCOUT
				testBeta = alpha+1;
#endif // SCOUT
			}
		}

		return alpha;
	}


	/*
	 * Help function for alphabeta
	 * used for jump-moves
	 */
	int Search::captureAlphaBeta(Board& board, int depth, int alpha, int beta, unsigned int from) {
		unsigned int moves = board.getCaptureMoves(from);
		int nrOfMoves = countBits(moves);
		unsigned int moveTo = 0x0u;
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
		if(countBits(board.getCaptureMoves(from)) == 0)
		{
			movement->push_back(to);
		}
		movement->push_back(from);
	}

	// Using heap-sort. try optimize as much as possible
	void Search::sortMoves(unsigned int movelist[], int movevalues[], unsigned int movecount)
	{
		int start = movecount /2;
		int end = movecount-1;
		while(start >= 0)
		{
			siftDown(movelist, movevalues, start, end);
			--start;
		}
		while(end > 0)
		{
			swap(movelist, movevalues, end, 0);
			end--;
			siftDown(movelist, movevalues, 0, end);
		}
		
	}

	inline void Search::siftDown(unsigned int movelist[], int movevalues[], int start, int end)
	{
		int root = start;
		while((root*2 + 1) <= end)
		{
			int child = root*2+1;
			if((child + 1 <= end) && (movevalues[child] < movevalues[child+1]))
			{
				child++;
			}
			if(movevalues[root] < movevalues[child])
			{
				swap(movelist, movevalues, root, child);
				root = child;
			}
			else
				return;
		}
	}

	inline void Search::swap(unsigned int movelist[], int movevalues[], int a, int b)
	{
		int tmp = movevalues[a];
		movevalues[a] = movevalues[b];
		movevalues[b] = tmp;

		unsigned int utmp = movelist[a<<1];
		movelist[a<<1] = movelist[b<<1];
		movelist[b<<1] = utmp;

		utmp = movelist[(a<<1) +1];
		movelist[(a<<1) +1] = movelist[(b<<1) +1];
		movelist[(b<<1) +1] = utmp;
	}

	void Search::sortTest() {
		unsigned int movelist[96] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};
		int movevalues[48] = {120,23,123,345,12,93,3,100,23,43};
		unsigned int movecount = 10;

		std::cout << "Osorterat:\n";
		for(int i = 0; i<10; i++) {
			std::cout << "From: " << movelist[i*2] << " To: " << movelist[i*2+1] << " Value: " << movevalues[i] << "\n";
		}
		sortMoves(movelist, movevalues, movecount);
		std::cout << "Sorterat:\n";
		for(int i = 0; i<10; i++) {
			std::cout << "From: " << movelist[i*2] << " To: " << movelist[i*2+1] << " Value: " << movevalues[i] << "\n";
		}
	}
}
