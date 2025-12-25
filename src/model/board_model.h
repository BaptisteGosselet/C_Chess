#ifndef BOARD_MODEL_H
#define BOARD_MODEL_H

#include "cell.h"
#include "color.h"
#include "move.h"
#include <stdbool.h>

extern Cell board_model[8][8];
extern const Cell EMPTY_CELL;
extern MoveList currentLegalMovesList;

extern bool whiteKingHasMoved;
extern bool whiteRookKHasMoved;
extern bool whiteRookQHasMoved;
extern bool whiteCastle;
extern bool blackKingHasMoved;
extern bool blackRookKHasMoved;
extern bool blackRookQHasMoved;
extern bool blackCastle;

void init_game();
void moveTo();


#endif 
