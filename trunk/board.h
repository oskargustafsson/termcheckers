#define WHITE 0x0
#define BLACK 0x1

struct board {
	unsigned int black_men;
	unsigned int white_men;
	unsigned int black_kings;
	unsigned int white_kings;
	bool player; // WHITE=0, BLACK=1
};

unsigned int getCaptureMoves(const board& b, unsigned int piece, bool king);
unsigned int getMoves(const board& b, unsigned int piece, bool king);

unsigned int upleft(unsigned const int& piece);
unsigned int upright(unsigned const int& piece);
unsigned int downleft(unsigned const int& piece);
unsigned int downright(unsigned const int& piece);

board createBoard();
