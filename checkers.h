
// some global variables
int currentplayer;

bool OUTPUT;
bool WHITE_AI;
bool BLACK_AI;
int WHITE_DEPTH;
int BLACK_DEPTH;

void user(board& b);
bool makeMove(board& b, unsigned int from, unsigned int to);
int newGame(board& b);
