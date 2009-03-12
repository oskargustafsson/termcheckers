#ifndef EVALUATION_H
#define EVALUATION_H

#include "board.h"

#define ALPHA_WIN 32000
#define ALPHA_DRAW 99999
#define ALPHA_MAX 2147483640

using namespace checkers;

int evaluate(Board&);

#endif
