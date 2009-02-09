#include <iostream>
#include "board.cc"
#include "ui.cc"

int main() {
	board b = createBoard();
	displayBoard(b);
	std::cout << "\n";
	unsigned int tmp = 0x400;
	debugPrintIntBoard(tmp);
	std::cout << "\n";
	debugPrintIntBoard(getMoves(b, tmp, true));
}
