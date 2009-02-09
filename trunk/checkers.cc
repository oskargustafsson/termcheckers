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
	unsigned int ett = 1;
	unsigned int all = b.blackmen;
	all = all | b.blackkings;
	all = all | b.whitemen;
	all = all | b.whitekings;

	for(int i=0; i < 32; i++) {
		if((i) % 8 == 0) {
			std::cout << "  ";
		}
		if((ett & (b.blackmen>>i)) != 0) {
			std::cout << " b";
		} else if((ett & (b.blackkings>>i)) != 0) {
			std::cout << " B";
		} else if((ett & (b.whitemen>>i)) != 0) {
			std::cout << " w";
		} else if((ett & (b.whitekings>>i)) != 0) {
			std::cout << " W";
		} else {
			std::cout << "  ";
		}
		std::cout << "  ";

		if((i+1) % 4 == 0) {
			std::cout << std::endl;
		}
	}
}

int main() {
	board b = createBoard();
	displayBoard(b);
}
