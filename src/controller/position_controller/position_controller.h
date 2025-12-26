#ifndef POSITION_CONTROLLER_H
#define POSITION_CONTROLLER_H

void init_game(Position *pos);
void init_board(Position *pos);
void updateLegalMoves(Position *pos);
void changeColorTurn(Position *pos);
void moveTo(Position *pos, int oX, int oY, int dX, int dY);

#endif