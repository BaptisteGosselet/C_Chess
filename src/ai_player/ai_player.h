#ifndef AI_PLAYER_H
#define AI_PLAYER_H

#include "../model/pieces_type.h"
#include "../model/position.h"
#include "../model/game.h"

PieceType aiChoosePieceToPromoteTo(Position *pos);
void letComputerPlay(Game *game);

#endif
