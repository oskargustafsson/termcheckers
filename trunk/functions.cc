#include "functions.h"
#include <cmath>

int countBits(unsigned int board) {
	return __builtin_popcount (board);
}

void compute_bits_in_char()
{
	unsigned int i ;
	for (i = 0; i < 256; i++)
		bits_in_char [i] = countBits(i) ;
	return ;
}

int countBits2(unsigned int n)
{
	//kräver att bits_in_char är initierad

	return bits_in_char [ n        & 0xffu]
		+  bits_in_char [(n >>  8) & 0xffu]
		+  bits_in_char [(n >> 16) & 0xffu]
		+  bits_in_char [(n >> 24) & 0xffu] ;
}

int bitToDec(unsigned int b) {
	return static_cast<int>(log2(b));
}
