#ifndef LEGAL_MOVE_H
#define LEGAL_MOVE_H

#include "move.h"
#include <stdbool.h>

bool isThisMoveLegal(Move wantedMove);
void updateAllLegalMoves(int color);

#endif 
