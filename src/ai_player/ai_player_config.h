#ifndef AI_PLAYER_CONFIG_H
#define AI_PLAYER_CONFIG_H

#define MATE_SCORE 100000
#define MIN_SCORE -999999
#define MAX_SCORE 999999

typedef enum {
    COMPUTER_FIRST_MOVE,
    COMPUTER_GREEDY,
    COMPUTER_MINMAX
} ComputerType;

#define COMPUTER_MODE COMPUTER_MINMAX

#endif
