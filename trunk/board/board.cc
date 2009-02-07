/*
 * board.cc
 */
#include "board.h"
#include<iostream>
#include<vector>

#define FRAME -2;
#define OFF -1;
#define EMPTY 0;
#define WM 1;
#define WK 2;
#define BM 3;
#define BK 4;

using namespace std;

namespace board {

  Board::Board() : squares(100), turn('b') {
    Board::setupBoard();
  }

  Board::~Board() {
    //
    // Nothing so far
    //
  }

  void Board::movePiece(int from, int to) {
    if(!isLegal(from, to)) { //Check so that requested move is legal
      cout << "Illegal move." << endl;
      return;
    }
    squares[to] = squares[from];
    squares[from] = 0;
  }

  void Board::switchTurn() {
    if(turn == 'b') turn = 'w';
    else turn = 'b';
  }

  void Board::printTurn() {
    if(turn == 'b') {
      cout << "Black's turn.";
    }
    else {
      cout << "White's turn.";
    }
    cout << endl;
  }

  bool Board::isLegal(int from, int to) {

    /*Check so that what we are trying to move is actually valid*/
    if(squares[from] < 1) {
      return false;
    }
    else if(turn == 'b') {
      if(squares[from] < 3) {
        return false;
      }
    }
    else if(turn == 'w') {
      if(squares[from] > 2) {
        return false;
      }
    }

    /*
    else if(squares[from] == 1 or squares[from] == 2) {
      color = w;
    }
    else {
      color = b;
    }
    */

    if(squares[to] != 0) return false; //Check so target square is empty

    /*everything is ok*/
    switchTurn();
    return true;
    /*
    else if((squares[to] != 3 || squares[to] != 4) && color == 'w') {
      return false;
    }
    else if((squares[to] != 1 || squares[to] != 2) && color == 'b') {
      return false;
    }
    */
  }

  void Board::setupBoard() {
    bool tmp = false;
    for(size_t i=0; i < squares.size(); ++i) {

      if(i<11) {
        squares[i] = FRAME;
      }
      else if(i>88) {
        squares[i] = FRAME;
      }
      else if((i%10)==0 || (i+1)%10==0) {
        squares[i] = FRAME;
      }
      else if(tmp) {
        squares[i] = EMPTY;
        tmp = false;
      }
      else {
        squares[i] = OFF;
        tmp = true;
      }

      if(i%10 == 0) {
        if(tmp) {
          tmp = false;
        }
        else{
          tmp = true;
        }
      }
    }

    for(size_t i=0; i < squares.size(); ++i) {
      if(squares[i] == 0) {
        if(i < 39) {
          squares[i] = BM;
        }
        if(i > 60) {
          squares[i] = WM;
        }
      }
    }
  }

  void Board::displayBoard() {
    for(size_t i = 0; i < squares.size(); ++i) {
      if(i % 10 == 0) {
        cout << endl;
      }
      if(squares[i] == -1) {
        cout << "\033[37;47m  \033[0m";
      }
      else if(squares[i] == -2) {
        cout << "\033[30;40m  \033[0m";
      }
      else if(squares[i] == 3) {
        cout << "\033[31;42m{}\033[0m";
      }
      else if(squares[i] == 1) {
        cout << "\033[37;42m{}\033[0m";
      }
      else { //EMPTY SQUARE
        cout << "\033[32;42m  \033[0m";
      }
    }
    cout << endl;
  }
}
