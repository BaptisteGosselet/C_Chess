#ifndef GAME_H
#define GAME_H

#include "position.h"   
#include "player_type.h"
#include "history.h"

typedef struct {
    Position position;
    PlayerType whitePlayerType;
    PlayerType blackPlayerType;
    MoveHistory history;
} Game;

#endif
