#ifndef BOARD_H
#define BOARD_H

#define WHITE 0x0
#define BLACK 0x1

struct board {
	unsigned int black;
	unsigned int white;
	unsigned int kings;
	bool player; // WHITE=0, BLACK=1
};

static int bits_in_char [256];		// för bit-count

unsigned int getCaptureMoves(const board& b, unsigned int piece);
unsigned int getRecursiveCaptureMoves(board& b, unsigned int piece);
unsigned int getMoves(const board& b, unsigned int piece);

unsigned int up_left(unsigned const int& piece);
unsigned int up_right(unsigned const int& piece);
unsigned int down_left(unsigned const int& piece);
unsigned int down_right(unsigned const int& piece);

board createBoard();

bool endOfGame(const board& b);

void move(board& b, unsigned int from, unsigned int to);

unsigned int getCaptureBit(unsigned int from, unsigned int to);

void changePlayer(board& b);

int countBits(unsigned int b);

int countBits2(unsigned int b);

void compute_bits_in_char();

unsigned int getPossibleMoves(board& b);

unsigned int getPossibleCaptureMoves(board b);

unsigned int getNextLevel(board b, unsigned int m);

bool empty(const board& b, unsigned int piece);

bool validateMove(board& b, unsigned int from, unsigned int to);

#endif
