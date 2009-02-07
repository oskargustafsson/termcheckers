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
    cout << "Move: ";
    cin >> from;
    cout << endl;
    cout << "to: ";
    cin >> to;
    cout << endl;

    board->movePiece(from, to);
  }
}

