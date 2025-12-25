#ifndef LEGAL_MOVE_H
#define LEGAL_MOVE_H

#include "move.h"
#include <stdbool.h>

bool isThisMoveLegal(Move wantedMove, MoveList* movesList);
void updateAllLegalMoves(int color, MoveList* movesList);

#endif 
