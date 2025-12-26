#ifndef BOARD_MODEL_H
#define BOARD_MODEL_H

#include "cell.h"
#include "color.h"
#include "move.h"
#include <stdbool.h>

extern Cell board_model[8][8];
extern const Cell EMPTY_CELL;
extern MoveList currentLegalMovesList;

extern bool whiteCanKingCastle;
extern bool whiteCanQueenCastle;
extern bool blackCanKingCastle;
extern bool blackCanQueenCastle;

void init_game();
void moveTo();


#endif 
