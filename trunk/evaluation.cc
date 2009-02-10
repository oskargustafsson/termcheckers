#include "evaluation.h"
#include "board.h"
#include <iostream>

// höga poäng är bra för svart, låga bra för vit
using namespace std;

int evaluate(board& b) {
	int score = 0;

	for(int i=0x1; i != 0; i = (i<<1)) {
		if((b.black & i) != 0) {
			score += 100;					// +100 för varje vanlig svart man
			if(((b.white|b.kings) & i) != 0)
				score += 50;				// ytterligare +50 om det är en kung
			if(getMoves(b, i) != 0)
				score += 20;				// +20 om han dessutom kan röra sig
		}

		if((b.white & i) != 0) {
			score -= 100;					// -100 för varje vanlig vit man
			if(((b.white|b.kings) & i) != 0) 
				score -= 50;				// ytterligare -50 om det är en kung
			if(getMoves(b, i) != 0)
				score -= 30;				// -20 om han dessutom kan röra sig
		}
	}
	
	if(((b.white & 0x1<<31) != 0 ) && ((b.black & 0x1<<27) != 0 )) score -= 10;		// svart har en man i "the dog hole" (dåligt för svart)
	
	if(((b.white & 0x1<<4) != 0 ) && ((b.black & 0x1) != 0 )) score += 10;		// vit har en man i "the dog hole"	(dåligt för vit)
	
	if(b.player == BLACK) score -= 3;		// +/- 3 beroende på vems tur det är
	else score += 3;
	
	score += b.black & 0x0000000F;			// upp till +32 för stark svart "back rating"
	score -= (b.white>>28) & 0x0000000F;	// upp till -32 för stark vit "back rating"
	
	return score;
}
