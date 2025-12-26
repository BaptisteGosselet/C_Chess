#include "../../model/move.h"
#include "../../model/pieces_type.h"
#include "../../model/color.h"
#include "../../model/position.h"
#include <stdbool.h>
#include <stdio.h>

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

void listSlidingMoves(Position *pos, int oI, int oJ, int di, int dj, Color color, MoveList *movesList) {
    int i = oI + di;
    int j = oJ + dj;

    while (i >= 0 && i <= 7 && j >= 0 && j <= 7) {
        Cell cell = pos->board_model[i][j];

        if (cell.piece == PIECE_NONE) {
            addMoveToList(oI, oJ, i, j, 0, movesList);
        } else {
            if (cell.color != color) {
                addMoveToList(oI, oJ, i, j, 0, movesList);
            }
            break;
        }
        i += di;
        j += dj;
    }
}

void listPawnMoves(Position *pos, int oI, int oJ, Color color, MoveList *movesList) {
    int dir = (color == COLOR_WHITE) ? -1 : 1;
    int startRow = (color == COLOR_WHITE) ? 6 : 1;
    int nextRow = oI + dir;

    if (nextRow < 0 || nextRow > 7) return;

    // Avancer d'une case
    if (pos->board_model[nextRow][oJ].piece == PIECE_NONE) {
        addMoveToList(oI, oJ, nextRow, oJ, 0, movesList);

        // Avancer de deux cases
        int doubleRow = oI + 2 * dir;
        if (oI == startRow && pos->board_model[doubleRow][oJ].piece == PIECE_NONE) {
            addMoveToList(oI, oJ, doubleRow, oJ, 0, movesList);
        }
    }

    // Captures
    for (int dj = -1; dj <= 1; dj += 2) {
        int j = oJ + dj;
        if (j >= 0 && j <= 7) {
            Cell target = pos->board_model[nextRow][j];
            if (target.piece != PIECE_NONE && target.color != color) {
                addMoveToList(oI, oJ, nextRow, j, 0, movesList);
            }
        }
    }

    // TODO en passant / promotion
}

void listRookMoves(Position *pos, int oI, int oJ, Color color, MoveList *movesList) {
    listSlidingMoves(pos, oI, oJ, -1, 0, color, movesList);
    listSlidingMoves(pos, oI, oJ, 1, 0, color, movesList);
    listSlidingMoves(pos, oI, oJ, 0, -1, color, movesList);
    listSlidingMoves(pos, oI, oJ, 0, 1, color, movesList);
}

void listBishopMoves(Position *pos, int oI, int oJ, Color color, MoveList *movesList) {
    listSlidingMoves(pos, oI, oJ, -1, -1, color, movesList);
    listSlidingMoves(pos, oI, oJ, -1, 1, color, movesList);
    listSlidingMoves(pos, oI, oJ, 1, -1, color, movesList);
    listSlidingMoves(pos, oI, oJ, 1, 1, color, movesList);
}

void listQueenMoves(Position *pos, int oI, int oJ, Color color, MoveList *movesList) {
    listRookMoves(pos, oI, oJ, color, movesList);
    listBishopMoves(pos, oI, oJ, color, movesList);
}

void listKnightMoves(Position *pos, int oI, int oJ, Color color, MoveList *movesList) {
    int offsets[8][2] = {
        {-2,-1}, {-2,1}, {-1,-2}, {-1,2},
        {1,-2}, {1,2}, {2,-1}, {2,1}
    };

    for (int k = 0; k < 8; k++) {
        int i = oI + offsets[k][0];
        int j = oJ + offsets[k][1];

        if (i >= 0 && i <= 7 && j >= 0 && j <= 7) {
            Cell cell = pos->board_model[i][j];
            if (cell.piece == PIECE_NONE || cell.color != color) {
                addMoveToList(oI, oJ, i, j, 0, movesList);
            }
        }
    }
}

void listKingCastleMove(Position *pos, int oI, int oJ, Color color, MoveList *movesList) {

    struct {
        int dir;          // +1 = petit roque, -1 = grand roque
        int rookCol;      
        bool canCastle;
    } roques[2] = {
        { 1, 7, (color==COLOR_WHITE)?pos->whiteCanKingCastle:pos->blackCanKingCastle },  // petit roque
        {-1, 0, (color==COLOR_WHITE)?pos->whiteCanQueenCastle:pos->blackCanQueenCastle} // grand roque
    };

    for(int k=0; k<2; k++) {
        int d = roques[k].dir;
        int rookCol = roques[k].rookCol;
        if(!roques[k].canCastle) continue;

        // Vérifier que les cases entre roi et tour sont vides
        int step = (d==1)?1:-1;
        bool pathClear = true;
        for(int j = oJ + step; j != rookCol; j += step) {
            if(pos->board_model[oI][j].piece != PIECE_NONE) {
                pathClear = false;
                break;
            }
        }
        if(!pathClear) continue;

        // Vérifier que les cases traversées par le roi ne sont pas attaquées
        bool safe = true;
        int kingEndCol = oJ + 2*d; 
        for(int j = oJ; j != kingEndCol + step; j += step) {
            if(countAttacksOnSquare(pos, oI, j, (color==COLOR_WHITE)?COLOR_BLACK:COLOR_WHITE) > 0) {
                safe = false;
                break;
            }
        }

        if(safe) {
            addMoveToList(oI, oJ, oI, kingEndCol, 0, movesList);
        }
    }
}

void listKingMoves(Position *pos, int oI, int oJ, Color color, MoveList *movesList) {
    for (int i = oI-1; i <= oI+1; i++) {
        for (int j = oJ-1; j <= oJ+1; j++) {
            if (i >= 0 && i <= 7 && j >= 0 && j <= 7 && !(i == oI && j == oJ)) {
                Cell cell = pos->board_model[i][j];
                if (cell.piece == PIECE_NONE || cell.color != color) {
                    addMoveToList(oI, oJ, i, j, 0, movesList);
                }
            }
        }
    }

    listKingCastleMove(pos, oI, oJ, color, movesList);
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
