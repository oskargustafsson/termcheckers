#include "functions.h"
#include <cmath>

int countBits(unsigned int board) {
		  return __builtin_popcount (board);
}

/*
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
*/

int bitToDec(unsigned int v) {
		  // OR (IF YOU KNOW v IS A POWER OF 2):
		  static const unsigned int b[] = {0xAAAAAAAA, 0xCCCCCCCC, 0xF0F0F0F0, 
					 0xFF00FF00, 0xFFFF0000};
		  register unsigned int r = (v & b[0]) != 0;
		  r |= ((v & b[0]) != 0) << 0;
		  r |= ((v & b[1]) != 0) << 1;
		  r |= ((v & b[2]) != 0) << 2;
		  r |= ((v & b[3]) != 0) << 3;

		return r;

}
