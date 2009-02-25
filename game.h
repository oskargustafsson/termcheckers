#ifndef GAME_H
#define GAME_H

#include "board.h"
#include <stack>
#include <vector>
#include "search.h"

#define NOT_PLAYING 0
#define PLAYING 1
#define QUIT 2

#define TOTAL_TIME 300000000

namespace checkers {
	class GUI;
	class TranspositionTable;

	class Game {
		public:
			Game();
			~Game();
			void setGUI(GUI* g);
			bool makeMove(std::vector<unsigned int>& movements);
			void newGame();
			void loadGame(char* file);
			void play();
			void stop();
			void ai();
			bool undoLastMove();
			void updateBoardHistory(Board&, Board&);
			size_t countHistoryMatches(Board&);

			Board board;
			int state;
#ifdef TRANS_TABLE
			TranspositionTable* trans_table;
#endif // TRANS_TABLE
		private:
			int recursiveCapture(Board tmpboard, unsigned int from, unsigned int to);

			Board boards[50];
			size_t board_count;
			std::stack<Board> history;
			bool output;
			GUI* gui;

			int blackTime;
			int whiteTime;
	};
}
#endif
