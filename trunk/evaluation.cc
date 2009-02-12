#include <iostream>
#include "evaluation.h"
#include "board.h"

// höga poäng är bra för svart, låga bra för vit
using namespace std;

int evaluate(board& b) {
	int score = 0;
unsigned int allPieces = b.black|b.white;

	for(int i=0x1; i != 0; i = (i<<1)) {
		if((b.black & i) != 0) {
			if(getMoves(b, i) != 0) {
				score += 20;				// +20 om han dessutom kan röra sig
//				if((i < 0x0000000F) && (i > 0x00000F))
//					score += 30;			// +30 om draget leder till att han blir kung
			}
		}

		if((b.white & i) != 0) {
			if(getMoves(b, i) != 0) {
				score -= 20;				// -20 om han dessutom kan röra sig
//				if((i < 0xF) && (i > 0x00F))
//					score -= 30;			// -30 om draget leder till att han blir kung
			}
		}
	}

	score += 100 * countBits(b.black);			// +100 per svart man (inkl kungar)
	score += 50 * countBits(b.black & b.kings);	// +50 för varje pjäs som även är kung

	score += 100 * countBits(b.white);			// +100 per vit man (inkl kungar)
	score += 50 * countBits(b.white & b.kings);	// +50 för varje pjäs som även är kung

	if(((b.black & 0x0F000000)<<4) & (~allPieces)) score += 40;	// +40 för svart vänsterdrag som ger kung
	if(((b.black & 0x0F000000)<<5) & (~allPieces)) score += 40;	// +40 för svart högerdrag som ger kung

	if(((b.black & 0x000000F0)>>4) & (~allPieces)) score -= 40; // -40 för vitt vänsterdrag som ger kung
	if(((b.black & 0x000000F0)>>5) & (~allPieces)) score -= 40; // -40 för vitt vänsterdrag som ger kung

	
	if(((b.white & 0x1<<31) != 0 ) && ((b.black & 0x1<<27) != 0 )) score -= 10;		// svart har en man i "the dog hole" (dåligt för svart)
	
	if(((b.white & 0x1<<4) != 0 ) && ((b.black & 0x1) != 0 )) score += 10;		// vit har en man i "the dog hole"	(dåligt för vit)
	
	if(b.player == BLACK) score -= 3;		// +/- 3 beroende på vems tur det är
	else score += 3;
	
	score += b.black & 0x0000000F;			// upp till +32 för stark svart "back rating"
	score -= (b.white>>28) & 0x0000000F;	// upp till -32 för stark vit "back rating"
	
	return score;
}
