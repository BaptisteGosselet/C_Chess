#include "../../model/player_type.h"

PlayerType paramWhitePlayerType = PLAYER_HUMAN;
PlayerType paramBlackPlayerType = PLAYER_HUMAN;


void changeWhitePlayerType(){
    if(paramWhitePlayerType == PLAYER_HUMAN){
        paramWhitePlayerType = PLAYER_COMPUTER;
    }
    else{
        paramWhitePlayerType = PLAYER_HUMAN;
    }
}

void changeBlackPlayerType(){
    if(paramBlackPlayerType == PLAYER_HUMAN){
        paramBlackPlayerType = PLAYER_COMPUTER;
    }
    else{
        paramBlackPlayerType = PLAYER_HUMAN;
    }
};
