/*
 *btest.cc
 */
#include "board.h"
#include<iostream>

using namespace board;
using namespace std;

/*
 *Main method that enables testing of the board.cc file.
 */
int main() {
  int from, to;

  Board* board = new Board;

  cout << "Type '1' to exit program." << endl;

  while(from != 1) {
    board->displayBoard();
    board->printTurn();
    cout << "Move: ";
    cin >> from;

    if(from == 1) { //check for exit message
      cout << "Bye." << endl;
      break;
    }

    cout << "to: ";
    cin >> to;


    board->movePiece(from, to);
  }
}

