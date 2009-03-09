#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <cmath>
#include <sstream>
#include <termios.h>
#include "gui.h"
#include "evaluation.h"
#include "game.h"
#include "board.h"
#include "functions.h"

using namespace std;

namespace checkers {

	GUI::GUI()
	{
		clearScreen();
		cout << "\033[?25l";
	}

	GUI::~GUI() {
		cout << "\033[?25h";
	}

	void GUI::clearScreen() {
		cout << "\033[2J";
	}

	void GUI::printBoard(const Board& board) {
		int row = 0;

		cout << "\033[2;H"; // place cursor at top-left corner

		for(int i=0; i < 32; i++) {
			if(i % 4 == 0) {
				cout << "\033[3G";
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

				printf("\n");
				row++;
				if(row == 2) {
					row = 0;
				} else {
					i -= 4;
				}
			}
		}
		cout << "\033[23;3H";
	}

	void GUI::printInfo(Game& game)
	{
		cout << "\033[2;37H"; // place cursor at position 2x37

		cout << "\033[K"; // clear the line
		cout << "\033[32m+-< Game >---------------------+\033[0m";
		cout << "\033[1B"; // move cursor down

		cout << "\033[37G"; // set cursor at column 37
		cout << "\033[K"; // clear the line
		cout << "\033[32m|\033[0m ";
		if(game.board.player == BLACK) {
			cout << "\033[36m";
		}
		cout << "Black (" << game.black->getTime()/1000000 << " sec)\033[0m";
		cout << "\033[32m\033[68G|\033[0m\033[1B";

		cout << "\033[37G"; // set cursor at column 37
		cout << "\033[K"; // clear the line
		cout << "\033[32m|\033[0m ";
		if(game.board.player == WHITE) {
			cout << "\033[36m";
		}
		cout << "White (" << game.white->getTime()/1000000 << " sec)\033[0m";
		cout << "\033[32m\033[68G|\033[0m\033[1B";

		cout << "\033[37G"; // set cursor at column 37
		cout << "\033[K"; // clear the line
		cout << "\033[32m|\033[0m";
		cout << "\033[32m\033[68G|\033[0m\033[1B";

		cout << "\033[37G\033[K";
		cout << "\033[32m|\033[0m Move #: " << game.move_count;
		cout << "\033[32m\033[68G|\033[0m\033[1B";

		cout << "\033[37G"; // set cursor at column 37
		cout << "\033[K"; // clear the line
		cout << "\033[32m|\033[0m";
		cout << "\033[32m\033[68G|\033[0m\033[1B";

		cout << "\033[37G"; // set cursor at column 37
		cout << "\033[K"; // clear the line
		cout << "\033[32m+-< Last move >----------------+\033[0m";
		cout << "\033[1B"; // move cursor down

		cout << "\033[37G"; // set cursor at column 37
		cout << "\033[K"; // clear the line
		cout << "\033[32m|\033[0m Alphabeta value: " << game.lastMove.value;
		cout << "\033[32m\033[68G|\033[0m\033[1B";

		cout << "\033[37G"; // set cursor at column 37
		cout << "\033[K"; // clear the line
		cout << "\033[32m|\033[0m Extended depth: " << game.lastMove.extendedDepth;
		cout << "\033[32m\033[68G|\033[0m\033[1B";

		cout << "\033[37G"; // set cursor at column 37
		cout << "\033[K"; // clear the line
		cout << "\033[32m|\033[0m Calculation depth: " << game.lastMove.depth;
		cout << "\033[32m\033[68G|\033[0m\033[1B";

		cout << "\033[37G"; // set cursor at column 37
		cout << "\033[K"; // clear the line
		cout << "\033[32m|\033[0m Evaluation: " << evaluate(game.board, 0);
		cout << "\033[32m\033[68G|\033[0m\033[1B";

		cout << "\033[37G"; // set cursor at column 37
		cout << "\033[K"; // clear the line
		cout << "\033[32m|\033[0m Nodes visited: " << game.lastMove.nodes;
		cout << "\033[32m\033[68G|\033[0m\033[1B";

		cout << "\033[37G"; // set cursor at column 37
		cout << "\033[K"; // clear the line
		cout << "\033[32m|\033[0m Calculation time: ";
		cout << (game.lastMove.time)/1000000;
		cout << " sec";
		cout << "\033[32m\033[68G|\033[0m\033[1B";

		cout << "\033[37G"; // set cursor at column 37
		cout << "\033[K"; // clear the line
		cout << "\033[32m|\033[0m";
		cout << "\033[32m\033[68G|\033[0m\033[1B";

		cout << "\033[37G"; // set cursor at column 37
		cout << "\033[K"; // clear the line
		cout << "\033[32m|\033[0m";
		cout << "\033[32m\033[68G|\033[0m\033[1B";

		cout << "\033[37G"; // set cursor at column 37
		cout << "\033[K"; // clear the line
		cout << "\033[32m+------------------------------+\033[0m";
	}

	void GUI::printLog()
	{
		cout << "\033[19;2H"; // set cursor at row 18
		for(int i=0; i<4; i++)
		{
			printf("\033[K\033[3G%s\n", messages[i].c_str());
		}
	}

	void GUI::printInt(unsigned int b)
	{
		for(int i=0; i < 32; i++)
		{
			if((i+4) % 8 != 0)
				printf("\033[47m  \033[0m");
			if((0x1 & (b>>i)) != 0)
				printf("\033[40m  \033[0m");
			else
				printf("\033[42m  \033[0m");
			if((i+1) % 4 == 0)
			{
				if((i+5)%8 != 0)
				{
					printf("\033[47m  \033[0m");
				}
				printf("\n");
			}
		}
		printf("\n");
	}

	void GUI::printWelcome()
	{
		cout << "\033[2;1H";
		cout << "        _                                _\n";
		cout << "       | |    ___ _ __  _ __   __ _ _ __| |_\n";
		cout << "       | |   / _ \\ '_ \\| '_ \\ / _` | '__| __|\n";
		cout << "       | |__|  __/ | | | | | | (_| | |  | |_\n";
		cout << "       |_____\\___|_| |_|_| |_|\\__,_|_|   \\__|\n";
		cout << "\n\n\n";
		cout << " Authors: Johan Ask, Linus Probert and Oskar Gustafsson.\n";
	}

	void GUI::println(string str)
	{
		for(int i=0; i<3; i++)
		{
			messages[i] = messages[i+1];
		}
		messages[3] = str;
		printLog();
	}

	int GUI::menu(string elements[], int size)
	{
		size_t width = 0;
		int choice = 0;
		unsigned char c;
		size_t x,y;

		for(int i = 0; i<size; i++)
		{
			if(elements[i].size() > width)
			{
				width = elements[i].size();
			}
		}

		x = 17 - width/2;
		y = 9 - size/2;

		do {
			cout << "\033[" << y << ";" << x << "H";

			cout << "\033[44;37m+";
			for(size_t col = 0; col < width+2; col++)
			{
				cout << "-";
			}
			cout << "+";
			for(int row = 0; row < size; row++)
			{
				cout << "\033[B";
				cout << "\033[" << x << "G";
				if(row == choice)  
					cout << "\033[41;37m|*";
				else
					cout << "\033[44;37m| ";
				for(size_t col = 0; col < width; col++)
				{
					if(col < elements[row].size())
						cout << elements[row][col];
					else
						cout << " ";
				}
				cout << " |\033[0m";
			}
			cout << "\033[B\033[" << x << "G\033[44;37m+";
			for(size_t col = 0; col < width+2; col++)
			{
				cout << "-";
			}
			cout << "+";

			cout << "\033[" << y+choice+1 << ";" << x << "H\033[0m";

			fflush(stdout);
			c = getch();

			if(c == 'j' || c == 'l' || c == 9 || c == 14)
			{
				if(choice < size-1)
					choice++;
				else
					choice = 0;
			}
			else if(c == 'k' || c == 'h' || c == 16)
			{
				if(choice > 0)
					choice--;
				else
					choice = size-1;
			}
		} while(c != 13);

		return choice;
	}

	bool GUI::dialogbox(string message)
	{
		size_t width = message.size();
		bool choice = true;
		unsigned char c;
		size_t x,y;

		x = 17 - width/2;
		y = 9;

		do {
			cout << "\033[" << y << ";" << x << "H";

			cout << "\033[44;37m+";
			for(size_t col = 0; col < width+2; col++)
			{
				cout << "-";
			}
			cout << "+";
			cout << "\033[B";
			cout << "\033[" << x << "G";
			cout << "\033[44;37m| ";
			cout << message;
			cout << " |";

			cout << "\033[B\033[" << x << "G|";
			if(choice)
			{
				cout << " \033[41;37m[Yes]\033[44;37m [No]";
			}
			else
			{
				cout << " \033[44;37m[Yes] \033[41;37m[No]";
			}
			for(size_t col = 9; col < width; col++)
			{
				cout << "\033[44;37m ";
			}
			cout << "|";

			cout << "\033[B\033[" << x << "G\033[44;37m+";
			for(size_t col = 0; col < width+2; col++)
			{
				cout << "-";
			}
			cout << "+\033[0m";

			fflush(stdout);
			c = getch();

			if(c == 'j' || c == 'l' || c == 'k' || c == 'h' || c == 9)
				choice = !choice;
		} while(c != 13);

		return choice;
	}

	void GUI::gameOver() {
		cout << "\033[8;H\033[37;41m";
		cout << "\033[8G +------------------+ \n";
		cout << "\033[8G | GAME OVER BITCH! | \n";
		cout << "\033[8G |    You Lose!     | \n";
		cout << "\033[8G +------------------+ \n";
		cout << "\033[0m";
	}

	string GUI::input()
	{
		string command;
		cout << "\033[23;1H>\033[K ";
		cout << "\033[?25h";
		cin >> command;
		cout << "\033[23;1H\033[K";
		cout << "\033[?25l";

		return command;
	}

	unsigned char GUI::getch()
	{
		// set TTY in one-character-at-a-time mode
		tcgetattr(STDIN_FILENO, &termattr);
		save_termattr = termattr;
		termattr.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
		termattr.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
		termattr.c_cflag &= ~(CSIZE | PARENB);
		termattr.c_cflag |= CS8;
		termattr.c_oflag &= ~(OPOST);
		termattr.c_cc[VMIN] = 1;
		termattr.c_cc[VTIME] = 0;
		tcsetattr (STDIN_FILENO, TCSANOW, &termattr);
		ttystate = RAW;
		ttysavefd = STDIN_FILENO;

		// read a character
		int i;
		unsigned char ch;
		ssize_t size;
		/*  termattr.c_cc[VMIN] = 0; */ /* uncomment if needed */
		i = tcsetattr (STDIN_FILENO, TCSANOW, &termattr);
		size = read (STDIN_FILENO, &ch, 1);
		/*  termattr.c_cc[VMIN] = 1; */ /* uncomment if needed */
		i = tcsetattr (STDIN_FILENO, TCSANOW, &termattr);

		// restore TTY mode
		tcsetattr (STDIN_FILENO, TCSAFLUSH, &save_termattr);
		ttystate = RESET;

		if (size == 0)
			return 0;
		else
			return ch;
	}

	void GUI::shadow(const Board& board, unsigned int from, unsigned int to)
	{
		bool king = (board.kings & to) != 0;
		int x=0, y=0, i=0;
		while(from != 0)
		{
			from = from>>1;
			i++;
		}
		y = ((i/4)+1)*2;
		x = -((i/4)%2)*4 + ((i-1)%4)*8 + 8;
		cout << "\033[" << y << ";" << x << "H\033[30;42m";
		if(king)
			cout << "WW";
		else
			cout << "==";
		cout << "\033[H";
		fflush(stdout);
	}

	void GUI::quit()
	{
		cout << "\033[0m\033[23;0H";
	}
}
