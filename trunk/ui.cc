#include "ui.h"
#include <cstdio>

void printBoard(const board b) {
	int row = 0;

	printf("\n\033[45m%36c\033[0m\n", ' ');
	for(int i=0; i < 32; i++) {
		if(i % 4 == 0) {
			printf("\033[45m%2c\033[0m", ' ');
		}
		if((i+4) % 8 != 0) {
			printf("\033[47m    \033[0m");
		}
		if(row == 0) {
			if((0x1 & (b.black>>i)) != 0) {
				printf("\033[42;31m BM \033[0m");
			} else if((0x1 & (b.white>>i)) != 0) {
				printf("\033[42;37m WM \033[0m");
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
			printf("\033[45m%2c\033[0m\n", ' ');
			row++;
			if(row == 2) {
				row = 0;
			} else {
				i -= 4;
			}
		}
	}
	printf("\033[45m%36c\033[0m\n", ' ');
}
