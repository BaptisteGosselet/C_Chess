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
} Position;

void init_game(Position *pos);
void init_board(Position *pos);
void updateLegalMoves(Position *pos);
void changeColorTurn(Position *pos);
void moveTo(Position *pos, int oX, int oY, int dX, int dY);

#endif
