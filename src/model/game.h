#ifndef GAME_H
#define GAME_H

#include "position.h"   
#include "player_type.h"

typedef struct {
    Position position;
    PlayerType whitePlayerType;
    PlayerType blackPlayerType;
} Game;

#endif
