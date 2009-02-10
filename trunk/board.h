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

unsigned int up_left(unsigned const int& piece);
unsigned int up_right(unsigned const int& piece);
unsigned int down_left(unsigned const int& piece);
unsigned int down_right(unsigned const int& piece);

board createBoard();

bool endOfGame(const board& b);

bool move(board& b, unsigned int from, unsigned int to);

void changePlayer(board& b);
