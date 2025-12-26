#ifndef GUI_H
#define GUI_H

#include "../model/pieces_type.h"
#include "../model/position.h"

void open_window(void);
void updateRender(void);

void awaitPromoteChoiceGUI(Position *pos, int dI, int dJ);
void confirmPromoteChoice(PieceType);

#endif 
