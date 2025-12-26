#ifndef LEGAL_MOVE_H
#define LEGAL_MOVE_H

#include "move.h"
#include "position.h"
#include "color.h"
#include <stdbool.h>

bool isThisMoveLegal(Move wantedMove, MoveList *movesList);
void updateAllLegalMoves(Position *pos, Color color, MoveList *movesList);

#endif
