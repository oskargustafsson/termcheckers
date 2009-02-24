#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <cmath>
#include "gui.h"
#include "evaluation.h"
#include "game.h"
#include "board.h"

using namespace std;

namespace checkers {

  GUI::GUI(Game* g) : game(g) {
    moveCount = 0;
    totalTime = 0;
    redraw = true;
  }

  GUI::~GUI() {
  }

  void GUI::clearScreen() {
    string clear;
    clear.append(80, ' ');
    cout << "\033[21A" << clear << endl;
    for(size_t i = 0; i < 20; ++i) {
      cout << clear << endl;
    }
    cout << endl;
  }

  void GUI::printBoard(Board& board) {
    int row = 0;

    clearScreen();

    printf("\n\033[21A\033[40m%36c\033[0m\n", ' ');
    for(int i=0; i < 32; i++) {
      if(i % 4 == 0) {
        printf("\033[40m%2c\033[0m", ' ');
      }
      if((i+4) % 8 != 0) {
        printf("\033[47m    \033[0m");
      }
      if(row == 0) {
        if((0x1 & (board.black>>i)) != 0) {
          if((0x1 & (board.kings>>i)) != 0)
            printf("\033[42;1;31m %c%c \033[0m", 'W', 'W');
          else
            printf("\033[42;1;31m %c%c \033[0m", '=', '=');
        } else if((0x1 & (board.white>>i)) != 0) {
          if((0x1 & (board.kings>>i)) != 0)
            printf("\033[42;1;37m %c%c \033[0m", 'W', 'W');
          else
            printf("\033[42;1;37m %c%c \033[0m", '=', '=');
        } else {
          printf("\033[42m    \033[0m");
        }
      } else if(row == 1) {
        printf("\033[42;30m %2d \033[0m", i+1);
      }

      if((i+1) % 4 == 0) {
        if((i+5)% 8 != 0) {
          printf("\033[47m    \033[0m");
        }


        printf("\033[40m%2c\033[0m", ' ');

        /*information output*/
        if(i == 3 && row == 0) {
          cout << "\033[32m+-< Game >-----------+\033[0m";
        }
        if(i == 3 && row == 1) {
          printf("\033[32m|\033[0mPlayer: ");
          if(board.player == BLACK) {
            printf("Black");
            player = "Black";
          } else {
            printf("White");
            player = "White";
          }
        }
        if(i == 7 && row == 0) {
          cout << "\033[32m|\033[0m";
        }
        if(i == 7 && row == 1) {
          cout << "\033[32m+-< AI >-------------+\033[0m";
        }
        if(i == 11 && row == 0) {
          cout << "\033[32m|\033[0m" << "Alphabeta value: " << value;
        }
        if(i == 11 && row == 1) {
          cout << "\033[32m|\033[0m" << "Extended depth: " << extdepth;
          extdepth = "0";
        }
        if(i == 15 && row == 1) {
          cout << "\033[32m|\033[0m" << "Calculation depth: " << depth;
          depth = "0";
        }
        if(i == 15 && row == 0) {
          cout << "\033[32m|\033[0m" << "Evaluation: " << evaluate(board);
        }
        if(i == 19 && row == 0) {
          cout << "\033[32m|\033[0m" << "Nodes visited: " << nodes;
          nodes = "0";
        }
        if(i == 19 && row == 1) {
          cout << "\033[32m|\033[0m" << "Calculation time: ";
          cout << std::strtod(timeUsed.c_str(), NULL)/1000000;
          cout << " sec";
          timeUsed = "0";
        }
        if(i == 23 && row == 0) {
          cout << "\033[32m|\033[0m" << "Total time: " << totalTime/1000000;
          cout << " sec";
        }
        if(i == 23 && row == 1) {
          cout << "\033[32m+-< General >--------+\033[0m";
        }
        if(i == 27 && row == 0) {
          cout << "\033[32m|\033[0mMove count: " << moveCount;
        }
         if(i == 27 && row == 1) {
          cout << "\033[32m|\033[0m";
        }
         if(i == 31 && row == 0) {
          cout << "\033[32m|\033[0m";
        }
        if(i == 31 && row == 1) {
          cout << "\033[32m|\033[0m" << lastMove;
        }//// info output done

        printf("\n");
        row++;
        if(row == 2) {
          row = 0;
        } else {
          i -= 4;
        }
      }
    }
    printf("\033[40m%36c\033[0m\n", ' ');

    // Log messages
    for(int i=0; i<4; i++)
    {
        printf("%s\n", messages[i].c_str());
    }
  }

  void GUI::printInt(unsigned int b) {
    for(int i=0; i < 32; i++) {
      if((i+4) % 8 != 0)
        printf("\033[47m  \033[0m");
      if((0x1 & (b>>i)) != 0)
        printf("\033[40m  \033[0m");
      else
        printf("\033[42m  \033[0m");
      if((i+1) % 4 == 0) {
        if((i+5)%8 != 0) {
          printf("\033[47m  \033[0m");
        }
        printf("\n");
      }
    }
    printf("\n");
  }

  void GUI::println(string str) {
    for(int i=0; i<3; i++)
    {
      messages[i] = messages[i+1];
    }
    messages[3] = str;
  }

  void GUI::setInfo(string str, string inf) {
    if(inf == "LM") {
      lastMove = str;
    }
    else if(inf == "TIME") {
      timeUsed = str;
      totalTime += std::strtol(str.c_str(), NULL, 10);
    }
    else if(inf == "NODES") {
      nodes = str;
    }
    else if(inf == "DEPTH") {
      depth = str;
    }
    else if(inf == "EXTDEPTH") {
      extdepth = str;
    }
    else if(inf == "VALUE") {
      value = str;
    }
    else if(inf == "ALPHAVAL") {
      alphaval = str;
    }
  }

  void GUI::gameOver() {
    redraw = false;
    cout << "\033[13A\033[37;41m";
    cout << "        +------------------+        " << endl;
    cout << "        | GAME OVER BITCH! |        " << endl;
    cout << "        |    You Lose!     |        " << endl;
    cout << "        +------------------+        ";
    cout << "\033[8B\033[0m" << endl;
    cout << "        \033[8D";
  }

  void GUI::stackLastMove() {
    std::pair<std::string,int> temp(lastMove, totalTime);
    moveHistory.push(temp);
  }

  void GUI::popLastMove() {
    std::pair<std::string, int> temp = moveHistory.top();
    moveHistory.pop();
    lastMove = temp.first;
    totalTime = temp.second;
  }

  void GUI::editMoveCounter(int b) {
    moveCount += b;
  }

  void GUI::input() {
    if(redraw)
      printBoard(game->board);
    redraw = true;

    string line;
    cout << "> ";
    cin >> line;

    switch (game->state) {
    case NOT_PLAYING:
      if(line == "play") {
        game->play();
      } else if(line == "newgame") {
        game->newGame();
      } else if(line == "help") {
        println("Commands: newgame, play, quit");
      } else if(line == "quit") {
        gameOver();
        game->state = QUIT;
      } else {
        println("Unknown command");
      }
      break;
    case PLAYING:
      int size;
      if((size = isMovement(line)) != 0)
        {
          vector<int unsigned> movement = parseMovement(line);
          game->makeMove(movement);
        }
      else if(line == "ai") {
        game->ai();
      }
      else if(line == "undo" || line == "back")
        {
          if(game->undoLastMove()) {
            println("Reverting!");
          } else {
            println("Nothing to undo!");
          }
        }
      else if(line == "help")
        {
          println("Commands: ai, undo, print, quit");
          println("Move: from-to");
        }
      else if(line == "quit" || line == "stop")
        {
          game->state = NOT_PLAYING;
          game->board.createBoard();
        } else if(line == "skip") {
        game->board.changePlayer();
        println("Skiping player");
      } else {
        println("Unknown command");
      }
      break;
    }
  }

  int GUI::isMovement(string line) {
    string::iterator It = line.begin();
    string tmpstr;
    int result = 1;
    while( It != line.end()) {
      if( *It == '-') {
        result++;
        if(atoi(tmpstr.c_str()) == 0) {
          result = 0;
          break;
        }
        tmpstr = "";
      } else
        tmpstr += *It;
      It++;
    }
    if(atof(tmpstr.c_str()) == 0) {
      result = 0;
    }
    return result;
  }

  vector<unsigned int> GUI::parseMovement(string line) {
    string::iterator It = line.begin();
    int i=0;
    vector<unsigned int> movement;
    string tmpstr;

    while( It != line.end()) {
      if(*It == '-') {
        movement.push_back(static_cast<unsigned int>(pow(2.0, atof(tmpstr.c_str())-1)));
        tmpstr = "";
        i++;
        It++;
        continue;
      }
      tmpstr += *It;
      It++;
    }
    movement.push_back(static_cast<unsigned int>(pow(2.0, atof(tmpstr.c_str())-1)));

    return movement;
  }
}
