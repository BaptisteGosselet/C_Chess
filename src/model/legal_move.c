#include "move.h"
#include "board_model.h"
#include "color.h"
#include <stdbool.h>
#include <stdio.h>

MoveList legalMovesList;

/**
 * Compare two move struct
 */
bool movesEqual(Move a, Move b){
    return 
        (a.fromX == b.fromX 
        && a.fromY == b.fromY 
        && a.toX   == b.toX 
        && a.toY   == b.toY 
        && a.promotion == b.promotion);
}

void addMoveToList(int fromX, int fromY, int toX, int toY, int promotion){    
    printf(
        "ADD MOVE : (%d,%d) -> (%d,%d) promo=%d\n",
        fromX, fromY, toX, toY, promotion
    );

    legalMovesList.moves[legalMovesList.count++] = (Move){
        .fromX = fromX,
        .fromY = fromY,
        .toX = toX,
        .toY = toY,
        .promotion = promotion
    };
}

void listPawnMoves(int oI, int oJ, Color color){

    if(color == COLOR_WHITE){
        if((oI-1)>=0 && board_model[oI-1][oJ].piece == EMPTY_CELL.piece){ 
            addMoveToList(oI, oJ, oI-1, oJ, 0);
        }
    }
    else if(color == COLOR_BLACK){
        if((oI+1)<=7 && board_model[oI+1][oJ].piece == EMPTY_CELL.piece){ 
            addMoveToList(oI, oJ, oI+1, oJ, 0);
        }
    }
    
    
    
    // black
    // todo : 2 cases au départ
    // todo : en passant
    // todo : capture
}

/*
void kingmoves
// roque = la case n'est pas dans legalMoves(inv(color)) 
// et il n'a pas déjà été produit par cette couleur

void listBishopMoves(int oI, int oJ, int color, MoveList *list){

    // coups diagonale HG
    // coups diagonale HD
    // coups diagonale BG
    // coups diagonale BD
}

void listRookMoves(int oI, int oJ, int color, MoveList *list){
        
    // Up moves
    for(int i=0; i<8; i++){}

    Move move = {
        .fromX = 0,
        .fromY = 0,
        .toX = 0,
        .toY = 0,
        .promotion = 0
    };
    // coups à gauche
    // coups à droite
    // coups en haut
    // coups en bas

    list->moves[list->count++] = move;
}

listQueenMoves(int oI, int oJ, int color, MoveList *list){
    // list tour + list fou
}
*/

/**
 * Empty the moves list
 */
void resetMovesList(){
    for(int i = 0; i < legalMovesList.count; i++){
        legalMovesList.moves[i] = (Move){0,0,0,0,0};
    }
    legalMovesList.count = 0;
}


/**
 * List all legal moves of the color 
 * 0 = white, 1 = black
 */
void updateAllLegalMoves(Color color) {
    printf("LIST MOVES\n");
    resetMovesList();
    for(int i=0; i<8; i++){
        for(int j=0; j<8; j++){
            if(legalMovesList.count >= MAX_MOVES){
                return;
            }

            if(board_model[i][j].color == color){
                listPawnMoves(i, j, color);
                /*
                if(board_model[i][j].piece == ){

                }
                */
            }
        }
    }
}

/**
 * check if the arg move is in the list
 */
bool isThisMoveLegal(Move wantedMove){
    bool isLegal = false;

    for (int i = 0; i < legalMovesList.count; i++) {
        Move m = legalMovesList.moves[i];
        if (movesEqual(m, wantedMove)) {
            isLegal = true;
            break;
        }
    }

    return isLegal;
}
