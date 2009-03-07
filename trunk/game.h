#ifndef GAME_H
#define GAME_H

#include "board.h"
#include "player.h"
#include <stack>
#include <vector>
#include <string>

#define NOT_PLAYING 0
#define PLAYING 1
#define QUIT 2

#define TOTAL_TIME 300000000

namespace checkers {
	class GUI;
	class Player;

	struct SearchResult {
		int nodes;
		int depth;
		int time;
		int value;
		int extendedDepth;
		std::vector<unsigned int> move;
	};

	class Game {
		public:
			Game(Board&, GUI*);
			~Game();
			void load(char* file);
			size_t countHistoryMatches(Board&);
			void interpretCommand(std::string);
			bool makeMove(std::vector<unsigned int>& movements);

			Board board;
			int move_count;
			SearchResult lastMove;

			Player* black;
			Player* white;
		private:
			int recursiveCapture(Board tmpboard, unsigned int from, unsigned int to);
			void ai();
			bool undo();
			void updateBoardHistory(Board&, Board&);

			int isMovement(std::string line);
			std::vector<unsigned int> parseMovement(std::string line);

			bool playing;
			Board boards[50];
			size_t board_count;
			std::stack<Board> history;
			bool output;
			GUI* gui;
	};
}

#endif // GAME_H
