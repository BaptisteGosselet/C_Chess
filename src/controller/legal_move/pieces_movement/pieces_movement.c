#include "../../../model/move.h"
#include "../../../model/pieces_type.h"
#include "../../../model/color.h"
#include "../../../model/position.h"
#include "../legal_move.h"
#include <stdbool.h>
#include <stdio.h>

void listSlidingMoves(Position *pos, int oI, int oJ, int di, int dj, Color color, MoveList *movesList) {
    int i = oI + di;
    int j = oJ + dj;

    while (i >= 0 && i <= 7 && j >= 0 && j <= 7) {
        Cell cell = pos->board_model[i][j];

        if (cell.piece == PIECE_NONE) {
            addMoveToList(oI, oJ, i, j, movesList);
        } else {
            if (cell.color != color) {
                addMoveToList(oI, oJ, i, j, movesList);
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
        addMoveToList(oI, oJ, nextRow, oJ, movesList);

        // Avancer de deux cases
        int doubleRow = oI + 2 * dir;
        if (oI == startRow && doubleRow >= 0 && doubleRow <= 7 &&
            pos->board_model[doubleRow][oJ].piece == PIECE_NONE) {
            addMoveToList(oI, oJ, doubleRow, oJ, movesList);
        }
    }

    // Captures normales
    for (int dj = -1; dj <= 1; dj += 2) {
        int j = oJ + dj;
        if (j >= 0 && j <= 7) {
            Cell target = pos->board_model[nextRow][j];
            if (target.piece != PIECE_NONE && target.color != color) {
                addMoveToList(oI, oJ, nextRow, j, movesList);
            }
        }
    }

    // Prise en passant
    if (color == COLOR_WHITE && oI == 3) {
        if (oJ - 1 >= 0 && pos->blackPushedPawn == oJ - 1) {
            addMoveToList(oI, oJ, nextRow, oJ - 1, movesList);
        }
        if (oJ + 1 <= 7 && pos->blackPushedPawn == oJ + 1) {
            addMoveToList(oI, oJ, nextRow, oJ + 1, movesList);
        }
    } else if (color == COLOR_BLACK && oI == 4) { // ligne 4 pour les noirs
        if (oJ - 1 >= 0 && pos->whitePushedPawn == oJ - 1) {
            addMoveToList(oI, oJ, nextRow, oJ - 1, movesList);
        }
        if (oJ + 1 <= 7 && pos->whitePushedPawn == oJ + 1) {
            addMoveToList(oI, oJ, nextRow, oJ + 1, movesList);
        }
    }
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
                addMoveToList(oI, oJ, i, j, movesList);
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
            addMoveToList(oI, oJ, oI, kingEndCol, movesList);
        }
    }
}

void listKingMoves(Position *pos, int oI, int oJ, Color color, MoveList *movesList) {
    for (int i = oI-1; i <= oI+1; i++) {
        for (int j = oJ-1; j <= oJ+1; j++) {
            if (i >= 0 && i <= 7 && j >= 0 && j <= 7 && !(i == oI && j == oJ)) {
                Cell cell = pos->board_model[i][j];
                if (cell.piece == PIECE_NONE || cell.color != color) {
                    addMoveToList(oI, oJ, i, j, movesList);
                }
            }
        }
    }

    listKingCastleMove(pos, oI, oJ, color, movesList);
}
