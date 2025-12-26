#ifndef MAIN_CONTROLLER_H
#define MAIN_CONTROLLER_H

#include "../model/position.h"
#include "../model/game.h"

void init_game();
void controllerInit(Position *pos);
void selectCell(int i, int j);

const Game* controllerGetGame(void);
const int* controllerGetSelectedCell(void);

#endif