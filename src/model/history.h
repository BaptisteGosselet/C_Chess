#ifndef HISTORY_H
#define HISTORY_H

#include "move.h"
#include "cell.h"

#define MAX_HISTORY 200

typedef struct {
    Move move;
    Cell captured;
    bool whiteCanKingCastle;
    bool whiteCanQueenCastle;
    bool blackCanKingCastle;
    bool blackCanQueenCastle;
    int whitePushedPawn;
    int blackPushedPawn;
    int halfmove_clock;
} HistoryEntry;

typedef struct {
    HistoryEntry entries[MAX_HISTORY];
    int count;
} MoveHistory;

#endif