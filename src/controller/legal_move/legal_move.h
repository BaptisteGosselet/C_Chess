#ifndef LEGAL_MOVE_H
#define LEGAL_MOVE_H

#include "../../model/move.h"
#include "../../model/position.h"
#include "../../model/color.h"
#include <stdbool.h>

bool isThisMoveLegal(Move wantedMove, MoveList *movesList);
void updateAllLegalMoves(Position *pos, Color color, MoveList *movesList);

#endif
