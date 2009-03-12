#ifndef GAME_H
#define GAME_H

#include "board.h"
#include "player.h"
#include "move.h"
#include <stack>
#include <vector>
#include <string>
#include <utility>

#define TOTAL_TIME 300000000

namespace checkers
{
	class GUI;
	class Player;

	enum ActionState {
		PLAYING, BLACK_WON, WHITE_WON, DRAW, QUIT
	};

	struct GameState {
		Board board;
		ActionState action;
		int move_count;
		int moves_since_man;
		int moves_since_capture;
		int black_time;
		int white_time;
	};

	class Game
	{
		public:
			Game(Board&, GUI*);
			~Game();
			void load(char* file);
			size_t countHistoryMatches(Board&);
			void interpretCommand(std::string);
			bool makeMove(Move&);

			Player* black;
			Player* white;

			GameState state;
			Move last_move;
		private:
			void ai();
			bool undo();
			void updateBoardHistory(Board&, Board&);
			void updateState(const Board&, const Move&);

			int isMovement(std::string line);

			Board boards[50];
			size_t board_count;
			std::stack< std::pair<GameState,Move> > history;
			GUI* gui;
	};
}

#endif // GAME_H
