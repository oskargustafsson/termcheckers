#include "ui.h"
#include <iostream>

void displayBoard(const board b) {
	unsigned int one = 0x1;

	for(int i=0; i < 32; i++) {
		if((i) % 8 != 0) {
			std::cout << "\033[47m  \033[0m";
		}
		if((one & (b.black_men>>i)) != 0) {
			std::cout << " b";
		} else if((one & (b.black_kings>>i)) != 0) {
			std::cout << " B";
		} else if((one & (b.white_men>>i)) != 0) {
			std::cout << " w";
		} else if((one & (b.white_kings>>i)) != 0) {
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

void debugPrintIntBoard(unsigned int b) {
	unsigned int one = 0x1;

	for(int i=0; i < 32; i++) {
		if((i) % 8 != 0) {
			std::cout << "\033[47m  \033[0m";
		}
		if((one & (b>>i)) != 0) {
			std::cout << " x";
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

