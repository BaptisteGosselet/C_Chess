#ifndef MOVE_H
#define MOVE_H

#define MAX_MOVES 256  // nombre arbitraire

typedef struct {
    int fromX, fromY;
    int toX, toY;
} Move;

typedef struct {
    Move moves[MAX_MOVES];
    int count;
} MoveList;

#endif 
