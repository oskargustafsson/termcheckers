#ifndef GAME_H
#define GAME_H

#include "board.h"
#include "player.h"
#include <stack>
#include <vector>
#include <string>

#define PLAYING 0
#define BLACK_WON 1
#define WHITE_WON 2
#define QUIT 3

#define TOTAL_TIME 300000000

namespace checkers
{
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

	class Game
	{
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

			int state;
			Player* black;
			Player* white;
		private:
			int recursiveCapture(Board, unsigned int from, unsigned int to, std::vector<unsigned int>&);
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
