#ifndef MAIN_CONTROLLER_H
#define MAIN_CONTROLLER_H

#include "../model/position.h"

void controllerInit(Position *pos);
void selectCell(int i, int j);

const Position* controllerGetPosition(void);
const int* controllerGetSelectedCell(void);


#endif