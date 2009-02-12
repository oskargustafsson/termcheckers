#ifndef GAME_H
#define GAME_H

#include "board.h"

namespace termcheckers {

	class Game {
	public:
		~Game();
		bool makeMove(unsigned int from, unsigned int to);
		void newGame(int wp, int bp);
		void play();
		static Game* instance();
		int currentplayer;
		int depth;
	private:
		Game();
		void user();

		static Game* game;
		board b;
		int black_player;
		int white_player;
	};
}
#endif
