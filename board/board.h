/*
 * board.h
 */
#include<vector>

using namespace std;

namespace board {
  /*
   *Board is the graphical view of a checkers board
   *it handles moves and checking if moves are valid.
   */
  class Board {
  public:

    /* Create the object that handles drawing of the board */
    Board();

    /*clearing the object and all the memory */
    ~Board();

    /*Move a piece from one square to the other*/
    void movePiece(int from, int to);

    /*Print a graphical view of the board in the terminal*/
    void displayBoard();

  private:
    /*Array for handling board squares and their contense*/
    vector<int> squares;

    /*Char indicating whos turn it is*/
    char turn;

    /*Switch turn to other player*/
    void switchTurn();

    /*Setup the board with starting positions of all peices.*/
    void setupBoard();

    /*Checks if the selected move is legal or not*/
    bool isLegal(int from, int to);
  };
}
