#ifndef MOVE_H
#define MOVE_H

#define MAX_MOVES 256  // nombre arbitraire

typedef struct {
    int fromX, fromY;
    int toX, toY;
    int promotion; //0=no prom, 1=queen, 2=rook, ...
} Move;

typedef struct {
    Move moves[MAX_MOVES];
    int count;
} MoveList;

#endif 
