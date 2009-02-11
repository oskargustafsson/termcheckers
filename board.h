#ifndef _board_
#define _board_

#define WHITE 0x0
#define BLACK 0x1

struct board {
	unsigned int black;
	unsigned int white;
	unsigned int kings;
	bool player; // WHITE=0, BLACK=1
};

unsigned int getCaptureMoves(const board& b, unsigned int piece);
unsigned int getMoves(const board& b, unsigned int piece);
unsigned int recursiveCapture(board b, unsigned int piece, unsigned int newpiece);

unsigned int up_left(unsigned const int& piece);
unsigned int up_right(unsigned const int& piece);
unsigned int down_left(unsigned const int& piece);
unsigned int down_right(unsigned const int& piece);

board createBoard();

bool endOfGame(const board& b);

void move(board& b, unsigned int from, unsigned int to);

unsigned int getCaptureBit(board& b, unsigned int from, unsigned int to);

void changePlayer(board& b);

int countBits(unsigned int b);

bool empty(const board& b, unsigned int piece);

#endif
