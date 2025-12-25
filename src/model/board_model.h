#ifndef BOARD_MODEL_H
#define BOARD_MODEL_H

#include "cell.h"
#include "color.h"
#include "move.h"

extern Cell board_model[8][8];
extern const Cell EMPTY_CELL;
extern MoveList currentLegalMovesList;


void init_game();
void moveTo();


#endif 
