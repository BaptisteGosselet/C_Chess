#include "cell.h"
#include "pieces_type.h"
#include "move.h"
#include "legal_move.h"
#include <stdio.h>
#include <stdbool.h>

const Cell EMPTY_CELL = (Cell){COLOR_EMPTY, PIECE_NONE};

Cell board_model[8][8];
Color currentColor;
MoveList currentLegalMovesList;
bool whiteCanKingCastle;
bool whiteCanQueenCastle;
bool blackCanKingCastle;
bool blackCanQueenCastle;



void init_board() {
    board_model[0][0] = (Cell){1,PIECE_ROOK};
    board_model[0][1] = (Cell){1,PIECE_KNIGHT};
    board_model[0][2] = (Cell){1,PIECE_BISHOP};
    board_model[0][3] = (Cell){1,PIECE_QUEEN};
    board_model[0][4] = (Cell){1,PIECE_KING};
    board_model[0][5] = (Cell){1,PIECE_BISHOP};
    board_model[0][6] = (Cell){1,PIECE_KNIGHT};
    board_model[0][7] = (Cell){1,PIECE_ROOK};

    for(int i=0;i<8;i++)
        board_model[1][i] = (Cell){1,PIECE_PAWN};

    for(int i=2;i<6;i++)
        for(int j=0;j<8;j++)
            board_model[i][j] = (Cell){-1,'\0'};

    for(int i=0;i<8;i++)
        board_model[6][i] = (Cell){0,PIECE_PAWN};

    board_model[7][0] = (Cell){0,PIECE_ROOK};
    board_model[7][1] = (Cell){0,PIECE_KNIGHT};
    board_model[7][2] = (Cell){0,PIECE_BISHOP};
    board_model[7][3] = (Cell){0,PIECE_QUEEN};
    board_model[7][4] = (Cell){0,PIECE_KING};
    board_model[7][5] = (Cell){0,PIECE_BISHOP};
    board_model[7][6] = (Cell){0,PIECE_KNIGHT};
    board_model[7][7] = (Cell){0,PIECE_ROOK};
}

void changeColorTurn(){
    if(currentColor == COLOR_WHITE){
        currentColor = COLOR_BLACK;
    }
    else{
        currentColor = COLOR_WHITE;
    }
}

void updateLegalMoves(){
    updateAllLegalMoves(currentColor, &currentLegalMovesList);
}

void init_game(){
    currentColor = COLOR_WHITE;

    whiteCanKingCastle = true; 
    whiteCanQueenCastle = true;
    blackCanKingCastle = true;
    blackCanQueenCastle = true;

    init_board();
    updateLegalMoves();
}

void checkMoveIndicator(int oX, int oY){
    if (board_model[oX][oY].piece == PIECE_KING) {
        if (currentColor == COLOR_WHITE){
            whiteCanKingCastle = false;
        }
        else if (currentColor == COLOR_BLACK){
            blackCanKingCastle = false;
        }
    }
    else if (board_model[oX][oY].piece == PIECE_ROOK) {
        if (oY == 0) {
            if (currentColor == COLOR_WHITE){
                whiteCanQueenCastle = false;
            }
            else if (currentColor == COLOR_BLACK){
                blackCanQueenCastle = false;
            }
        }
        else if (oY == 7) {
            if (currentColor == COLOR_WHITE){
                whiteCanKingCastle = false;
            }
            else if (currentColor == COLOR_BLACK){
                blackCanKingCastle = false;
            }
        }
    }



}

void handleCastleRook(int oY, int dX, int dY){
    if (board_model[dX][dY].piece == PIECE_KING){
        //Petit roque
        if(dY-oY==2){
            board_model[dX][dY-1] = board_model[dX][dY+1];
            board_model[dX][dY+1] = EMPTY_CELL;  
            if(board_model[dX][dY].color == COLOR_WHITE){
                whiteCanKingCastle = false;
            }   
            else if(board_model[dX][dY].color == COLOR_BLACK){
                blackCanKingCastle = false;
            }   
        } 
    
        // Grand roque
        else if(oY - dY == 2){
            board_model[dX][dY+1] = board_model[dX][dY-2];
            board_model[dX][dY-2] = EMPTY_CELL;

            if(board_model[dX][dY].color == COLOR_WHITE){
                whiteCanQueenCastle = false;
            }   
            else if(board_model[dX][dY].color == COLOR_BLACK){
                blackCanQueenCastle = false;
            }   
        }    
    }
}

void moveTo(int oX, int oY, int dX, int dY){
    if (oX<0 || oX>7 || oY<0 || oY>7 || dX<0 || dX>7 || dY<0 || dY>7) return;
    checkMoveIndicator(oX, oY);
    board_model[dX][dY] = board_model[oX][oY];
    board_model[oX][oY] = EMPTY_CELL;       
    handleCastleRook(oY, dX, dY);
    changeColorTurn();
    updateLegalMoves();
}