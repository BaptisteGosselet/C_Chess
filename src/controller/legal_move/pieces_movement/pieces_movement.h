#ifndef PIECES_MOVEMENT_H
#define PIECES_MOVEMENT_H

#include "../../../model/move.h"
#include "../../../model/position.h"
#include "../../../model/color.h"
#include <stdbool.h>


void listSlidingMoves(Position *pos, int oI, int oJ, int di, int dj, Color color, MoveList *movesList);
void listPawnMoves(Position *pos, int oI, int oJ, Color color, MoveList *movesList);
void listRookMoves(Position *pos, int oI, int oJ, Color color, MoveList *movesList);   
void listBishopMoves(Position *pos, int oI, int oJ, Color color, MoveList *movesList);
void listQueenMoves(Position *pos, int oI, int oJ, Color color, MoveList *movesList);   
void listKnightMoves(Position *pos, int oI, int oJ, Color color, MoveList *movesList);    
void listKingCastleMove(Position *pos, int oI, int oJ, Color color, MoveList *movesList);
void listKingMoves(Position *pos, int oI, int oJ, Color color, MoveList *movesList);
  
    
    
    

#endif

