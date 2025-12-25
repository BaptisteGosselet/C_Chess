#include "move.h"
#include "board_model.h"
#include "pieces_type.h"
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
        if(oI == 6
            && (oI-2)>=0 
            && board_model[oI-1][oJ].piece == EMPTY_CELL.piece
            && board_model[oI-2][oJ].piece == EMPTY_CELL.piece){ 
            addMoveToList(oI, oJ, oI-2, oJ, 0);
        }

        if((oI-1)>=0 && board_model[oI-1][oJ].piece == EMPTY_CELL.piece){ 
            addMoveToList(oI, oJ, oI-1, oJ, 0);
        }

        if((oI-1)>=0 && (oJ-1)>=0 && board_model[oI-1][oJ-1].color == COLOR_BLACK){ 
            addMoveToList(oI, oJ, oI-1, oJ-1, 0);
        }
        if((oI-1)>=0 && (oJ+1)<=7 && board_model[oI-1][oJ+1].color == COLOR_BLACK){ 
            addMoveToList(oI, oJ, oI-1, oJ+1, 0);
        }

    }
    else if(color == COLOR_BLACK){
        if(oI == 1
            && (oI+2)<=7 
            && board_model[oI+1][oJ].piece == EMPTY_CELL.piece
            && board_model[oI+2][oJ].piece == EMPTY_CELL.piece){ 
            addMoveToList(oI, oJ, oI+2, oJ, 0);
        }

        if((oI+1)<=7 && board_model[oI+1][oJ].piece == EMPTY_CELL.piece){ 
            addMoveToList(oI, oJ, oI+1, oJ, 0);
        }

        if((oI+1)<=7 && (oJ-1)>=0 && board_model[oI+1][oJ-1].color == COLOR_WHITE){ 
            addMoveToList(oI, oJ, oI+1, oJ-1, 0);
        }
        if((oI+1)<=7 && (oJ+1)<=7 && board_model[oI+1][oJ+1].color == COLOR_WHITE){ 
            addMoveToList(oI, oJ, oI+1, oJ+1, 0);
        }
    }
    
    // todo : en passant
}

void listRookMoves(int oI, int oJ, Color color){

    // Haut
    for(int i = oI-1; i >= 0; i--){
        if(board_model[i][oJ].piece == EMPTY_CELL.piece){
            addMoveToList(oI, oJ, i, oJ, 0);
        } else {
            if(board_model[i][oJ].color != color){
                addMoveToList(oI, oJ, i, oJ, 0); 
            }
            break; 
        }
    }

    // Bas
    for(int i = oI+1; i <= 7; i++){
        if(board_model[i][oJ].piece == EMPTY_CELL.piece){
            addMoveToList(oI, oJ, i, oJ, 0);
        } else {
            if(board_model[i][oJ].color != color){
                addMoveToList(oI, oJ, i, oJ, 0);
            }
            break;
        }
    }

    // Gauche
    for(int j = oJ-1; j >= 0; j--){
        if(board_model[oI][j].piece == EMPTY_CELL.piece){
            addMoveToList(oI, oJ, oI, j, 0);
        } else {
            if(board_model[oI][j].color != color){
                addMoveToList(oI, oJ, oI, j, 0); 
            }
            break;
        }
    }

    // Droite
    for(int j = oJ+1; j <= 7; j++){
        if(board_model[oI][j].piece == EMPTY_CELL.piece){
            addMoveToList(oI, oJ, oI, j, 0);
        } else {
            if(board_model[oI][j].color != color){
                addMoveToList(oI, oJ, oI, j, 0); 
            }
            break;
        }
    }
}

void listBishopMoves(int oI, int oJ, Color color){
    // Diagonale Haut-Gauche
    for(int i = oI-1, j = oJ-1; i >= 0 && j >= 0; i--, j--){
        if(board_model[i][j].piece == EMPTY_CELL.piece){
            addMoveToList(oI, oJ, i, j, 0);
        } else {
            if(board_model[i][j].color != color){
                addMoveToList(oI, oJ, i, j, 0); // capture
            }
            break;
        }
    }

    // Diagonale Haut-Droite
    for(int i = oI-1, j = oJ+1; i >= 0 && j <= 7; i--, j++){
        if(board_model[i][j].piece == EMPTY_CELL.piece){
            addMoveToList(oI, oJ, i, j, 0);
        } else {
            if(board_model[i][j].color != color){
                addMoveToList(oI, oJ, i, j, 0);
            }
            break;
        }
    }

    // Diagonale Bas-Gauche
    for(int i = oI+1, j = oJ-1; i <= 7 && j >= 0; i++, j--){
        if(board_model[i][j].piece == EMPTY_CELL.piece){
            addMoveToList(oI, oJ, i, j, 0);
        } else {
            if(board_model[i][j].color != color){
                addMoveToList(oI, oJ, i, j, 0);
            }
            break;
        }
    }

    // Diagonale Bas-Droite
    for(int i = oI+1, j = oJ+1; i <= 7 && j <= 7; i++, j++){
        if(board_model[i][j].piece == EMPTY_CELL.piece){
            addMoveToList(oI, oJ, i, j, 0);
        } else {
            if(board_model[i][j].color != color){
                addMoveToList(oI, oJ, i, j, 0);
            }
            break;
        }
    }
}

void listQueenMoves(int oI, int oJ, Color color){
    listRookMoves(oI, oJ, color);  
    listBishopMoves(oI, oJ, color);  
}

void listKingMoves(int oI, int oJ, Color color){
    for(int i = oI-1; i <= oI+1; i++){
        for(int j = oJ-1; j <= oJ+1; j++){
            if(i >= 0 && i <= 7 && j >= 0 && j <= 7 && !(i == oI && j == oJ)){
                if(board_model[i][j].piece == EMPTY_CELL.piece || board_model[i][j].color != color){
                    addMoveToList(oI, oJ, i, j, 0);
                }
            }
        }
    }

    // TODO : roques, 
    // TODO : gestion des echecs
}

void listKnightMoves(int oI, int oJ, Color color){
    int moves[8][2] = {
        {-2, -1}, {-2, +1},
        {-1, -2}, {-1, +2},
        {+1, -2}, {+1, +2},
        {+2, -1}, {+2, +1}
    };

    for(int k = 0; k < 8; k++){
        int i = oI + moves[k][0];
        int j = oJ + moves[k][1];

        if(i >= 0 && i <= 7 && j >= 0 && j <= 7){
            if(board_model[i][j].piece == EMPTY_CELL.piece || board_model[i][j].color != color){
                addMoveToList(oI, oJ, i, j, 0);
            }
        }
    }
}


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
                
                
                if(board_model[i][j].piece == PIECE_ROOK){
                    listRookMoves(i, j, color);
                }
                else if(board_model[i][j].piece == PIECE_BISHOP){
                    listBishopMoves(i, j, color);
                }
                else if(board_model[i][j].piece == PIECE_KNIGHT){
                    listKnightMoves(i, j, color);
                }
                else if(board_model[i][j].piece == PIECE_QUEEN){
                    listQueenMoves(i, j, color);
                }
                else if(board_model[i][j].piece == PIECE_KING){
                    listKingMoves(i, j, color);
                }
                else if(board_model[i][j].piece == PIECE_PAWN){
                    listPawnMoves(i, j, color);
                }
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
