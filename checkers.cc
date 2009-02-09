#include "checkers.h"
#include <iostream>

board createBoard() {
	board b;
	b.blackmen = 0xFFF00000;
	b.whitemen = 0x00000FFF;
	b.blackkings = 0x0;
	b.whitekings = 0x0;
	return b;
}

void displayBoard(const board b) {
	unsigned int one = 0x1;

	for(int i=0; i < 32; i++) {
		if((i) % 8 != 0) {
			std::cout << "\033[47m  \033[0m";
		}
		if((one & (b.blackmen>>i)) != 0) {
			std::cout << " b";
		} else if((one & (b.blackkings>>i)) != 0) {
			std::cout << " B";
		} else if((one & (b.whitemen>>i)) != 0) {
			std::cout << " w";
		} else if((one & (b.whitekings>>i)) != 0) {
			std::cout << " W";
		} else {
			std::cout << "  ";
		}

		if((i+1) % 4 == 0) {
			if((i+1)% 8 != 0) {
				std::cout << "\033[47m  \033[0m";
			}
			std::cout << std::endl;
		}
	}
}

int main() {
	board b = createBoard();
	displayBoard(b);
}

int getMoves(const board& b, int piece) {
	int moves = 0x0;
	switch(b.player) {
		case WHITE:
			break;
		case BLACK:
			break;
	}
	return moves;
}
