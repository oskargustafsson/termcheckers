#ifndef GUI_H
#define GUI_H

#include <string>
#include <vector>
#include "board.h"
#include "game.h"

#define DEPTH 10

namespace checkers {
  class Game;

  class GUI {
  public:
    GUI(Game* g);
    ~GUI();

    void setInfo(std::string str, std::string inf);
    void printBoard(Board& b);
    static void printInt(unsigned int b);
    void input();
    void editMoveCounter(int b);
    void println(std::string str);

  private:
    Game* game;
    int isMovement(std::string line);
    std::vector<unsigned int> parseMovement(std::string line);

    void clearScreen();
    int moveCount;
    std::string lastMove;
  };
}

#endif
