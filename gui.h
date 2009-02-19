#ifndef GUI_H
#define GUI_H

#include <string>
#include <vector>
#include <stack>
#include "board.h"
#include "game.h"

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
    void popLastMove();
    void stackLastMove();
    void gameOver();

  private:
    Game* game;
    int isMovement(std::string line);
    std::vector<unsigned int> parseMovement(std::string line);

    void clearScreen();

    int moveCount;

    bool redraw;

    std::string player;
    std::string depth;
    std::string timeUsed;
    std::string nodes;
    std::string value;
    std::string lastMove;
    std::stack<std::string> moveHistory;
  };
}

#endif
