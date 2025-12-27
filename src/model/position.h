#ifndef POSITION_H
#define POSITION_H

#include <stdbool.h>
#include "cell.h"
#include "move.h"

#include "pieces_type.h"   

#define EMPTY_CELL (Cell){COLOR_EMPTY, PIECE_NONE}

typedef struct {
    Cell board_model[8][8];
    Color currentColor;
    MoveList currentLegalMovesList;
    bool whiteCanKingCastle;
    bool whiteCanQueenCastle;
    bool blackCanKingCastle;
    bool blackCanQueenCastle;
    bool isFinal;
    int whitePushedPawn;
    int blackPushedPawn;
    int move_count;
    int halfmove_clock;
} Position;

#endif
