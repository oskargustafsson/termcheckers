#define WHITE 0;
#define BLACK 1;

struct board {
	unsigned int blackmen;
	unsigned int whitemen;
	unsigned int blackkings;
	unsigned int whitekings;
	bool player; // WHITE=0, BLACK=1
};


int getKingCaptureMoves(const board b&, int piece);
int getCaptureMoves(const board b&, int piece);
int getKingMoves(const board b&, int piece);
int getMoves(const board b&, int piece);

int evaluate(const board b&);

board alphabeta(board b);

void displayBoard(const board b);

board createBoard();
