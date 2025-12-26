#include "../../model/move.h"
#include "../../model/pieces_type.h"
#include "../../model/color.h"
#include "../../model/position.h"
#include <stdbool.h>
#include <stdio.h>
#include "pieces_movement/pieces_movement.h"
/**
 * Retourne combien de fois la case (x, y) est attaquée par la couleur `byColor`
 */
int countAttacksOnSquare(Position *pos, int x, int y, Color byColor) {
    int count = 0;

    for(int i=0; i<8; i++){
        for(int j=0; j<8; j++){
            Cell cell = pos->board_model[i][j];
            if(cell.color != byColor) continue;

            switch(cell.piece){
                case PIECE_PAWN: {
                    int dir = (byColor == COLOR_WHITE) ? -1 : 1;
                    for(int dj=-1; dj<=1; dj+=2){
                        int nx = i + dir;
                        int ny = j + dj;
                        if(nx==x && ny==y) count++;
                    }
                    break;
                }
                case PIECE_KNIGHT: {
                    int offsets[8][2] = {{-2,-1},{-2,1},{-1,-2},{-1,2},{1,-2},{1,2},{2,-1},{2,1}};
                    for(int k=0;k<8;k++){
                        int nx=i+offsets[k][0], ny=j+offsets[k][1];
                        if(nx==x && ny==y) count++;
                    }
                    break;
                }
                case PIECE_BISHOP:
                case PIECE_ROOK:
                case PIECE_QUEEN: {
                    int dirs[8][2] = {
                        {-1,-1},{-1,1},{1,-1},{1,1}, // diagonales
                        {-1,0},{1,0},{0,-1},{0,1}    // lignes/colonnes
                    };
                    int maxDir = (cell.piece == PIECE_BISHOP) ? 4 :
                                 (cell.piece == PIECE_ROOK) ? 4 : 8;
                    for(int d=0; d<maxDir; d++){
                        int dx = dirs[d][0], dy = dirs[d][1];
                        int nx = i+dx, ny = j+dy;
                        while(nx>=0 && nx<8 && ny>=0 && ny<8){
                            Cell c = pos->board_model[nx][ny];
                            if(nx==x && ny==y){
                                count++;
                                break;
                            }
                            if(c.piece != PIECE_NONE) break;
                            nx+=dx; ny+=dy;
                        }
                    }
                    break;
                }
                case PIECE_KING: {
                    for(int dx=-1; dx<=1; dx++){
                        for(int dy=-1; dy<=1; dy++){
                            if(dx==0 && dy==0) continue;
                            if(i+dx==x && j+dy==y) count++;
                        }
                    }
                    break;
                }
                default: break;
            }
        }
    }

    return count;
}


bool movesEqual(Move a, Move b) {
    return a.fromX == b.fromX
        && a.fromY == b.fromY
        && a.toX   == b.toX
        && a.toY   == b.toY
        && a.promotion == b.promotion;
}

void addMoveToList(int fromX, int fromY, int toX, int toY, int promotion, MoveList *movesList) {
    if (movesList->count >= MAX_MOVES) return;
    printf(
        "ADD MOVE [%d] : (%d,%d) -> (%d,%d) promotion=%d\n",
        movesList->count,
        fromX, fromY,
        toX, toY,
        promotion
    );
    movesList->moves[movesList->count++] = (Move){
        .fromX = fromX,
        .fromY = fromY,
        .toX = toX,
        .toY = toY,
        .promotion = promotion
    };
}

void resetMovesList(MoveList *movesList) {
    movesList->count = 0;
}

void updateAllLegalMoves(Position *pos, Color color, MoveList *movesList) {
    resetMovesList(movesList);

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            Cell cell = pos->board_model[i][j];

            if (cell.color == color) {
                switch (cell.piece) {
                    case PIECE_PAWN:   listPawnMoves(pos, i, j, color, movesList); break;
                    case PIECE_ROOK:   listRookMoves(pos, i, j, color, movesList); break;
                    case PIECE_BISHOP: listBishopMoves(pos, i, j, color, movesList); break;
                    case PIECE_KNIGHT: listKnightMoves(pos, i, j, color, movesList); break;
                    case PIECE_QUEEN:  listQueenMoves(pos, i, j, color, movesList); break;
                    case PIECE_KING:   listKingMoves(pos, i, j, color, movesList); break;
                    default: break;
                }
            }
        }
    }
}

bool isThisMoveLegal(Move wantedMove, MoveList *movesList) {
    for (int i = 0; i < movesList->count; i++) {
        if (movesEqual(movesList->moves[i], wantedMove)) {
            return true;
        }
    }
    return false;
}