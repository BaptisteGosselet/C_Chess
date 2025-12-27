#ifndef LEGAL_MOVE_H
#define LEGAL_MOVE_H

#include "../../model/move.h"
#include "../../model/position.h"
#include "../../model/color.h"
#include <stdbool.h>

bool isThisMoveLegal(Move wantedMove, MoveList *movesList);
void updateAllLegalMoves(Position *pos, Color color, MoveList *movesList);
int countAttacksOnSquare(Position *pos, int x, int y, Color byColor);
void addMoveToList(int fromX, int fromY, int toX, int toY, MoveList *movesList);
bool isKingInCheck(const Position *pos, Color color);
bool getKingPosition(const Position *pos, Color color, int *kingX, int *kingY);
void makeMove(Position *pos, Move move, Cell *captured);
void undoMove(Position *pos, Move move, Cell captured);

#endif
