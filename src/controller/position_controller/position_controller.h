#ifndef POSITION_CONTROLLER_H
#define POSITION_CONTROLLER_H

#include "../../model/game.h"

void init_position(Position *pos);
void init_board(Position *pos);
void updateLegalMoves(Position *pos);
void changeColorTurn(Position *pos);
void letComputerPlay(Game *game);
void moveTo(Game *game, int oX, int oY, int dX, int dY);

#endif