#include "move.h"
#include "pieces_type.h"
#include "color.h"
#include "position.h"
#include <stdbool.h>
#include <stdio.h>

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

    if (color == COLOR_WHITE) {

        if (pos->whiteCanKingCastle &&
            pos->board_model[oI][oJ+1].piece == PIECE_NONE &&
            pos->board_model[oI][oJ+2].piece == PIECE_NONE &&
            pos->board_model[oI][oJ+3].piece == PIECE_ROOK &&
            pos->board_model[oI][oJ+3].color == COLOR_WHITE) {

            addMoveToList(oI, oJ, oI, oJ+2, 0, movesList);
        }

        if (pos->whiteCanQueenCastle &&
            pos->board_model[oI][oJ-1].piece == PIECE_NONE &&
            pos->board_model[oI][oJ-2].piece == PIECE_NONE &&
            pos->board_model[oI][oJ-3].piece == PIECE_NONE &&
            pos->board_model[oI][oJ-4].piece == PIECE_ROOK &&
            pos->board_model[oI][oJ-4].color == COLOR_WHITE) {

            addMoveToList(oI, oJ, oI, oJ-2, 0, movesList);
        }
    }

    if (color == COLOR_BLACK) {

        if (pos->blackCanKingCastle &&
            pos->board_model[oI][oJ+1].piece == PIECE_NONE &&
            pos->board_model[oI][oJ+2].piece == PIECE_NONE &&
            pos->board_model[oI][oJ+3].piece == PIECE_ROOK &&
            pos->board_model[oI][oJ+3].color == COLOR_BLACK) {

            addMoveToList(oI, oJ, oI, oJ+2, 0, movesList);
        }

        if (pos->blackCanQueenCastle &&
            pos->board_model[oI][oJ-1].piece == PIECE_NONE &&
            pos->board_model[oI][oJ-2].piece == PIECE_NONE &&
            pos->board_model[oI][oJ-3].piece == PIECE_NONE &&
            pos->board_model[oI][oJ-4].piece == PIECE_ROOK &&
            pos->board_model[oI][oJ-4].color == COLOR_BLACK) {

            addMoveToList(oI, oJ, oI, oJ-2, 0, movesList);
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
