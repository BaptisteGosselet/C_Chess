#include "../../model/move.h"
#include "../../model/pieces_type.h"
#include "../../model/color.h"
#include "../../model/position.h"
#include <stdbool.h>
#include <stdio.h>
#include "pieces_movement/pieces_movement.h"
#include <string.h> 

/**
 * Retourne combien de fois la case (x, y) est attaquée par la couleur `byColor`
 */
int countAttacksOnSquare(const Position *pos, int x, int y, Color byColor) {
    int count = 0;

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            Cell cell = pos->board_model[i][j];
            if (cell.color != byColor) continue;

            switch(cell.piece) {
                case PIECE_PAWN: {
                    int dir = (byColor == COLOR_WHITE) ? -1 : 1;
                    for(int dj = -1; dj <= 1; dj += 2) {
                        int nx = i + dir;
                        int ny = j + dj;
                        if(nx == x && ny == y) count++;
                    }
                    break;
                }
                case PIECE_KNIGHT: {
                    int offsets[8][2] = {{-2,-1},{-2,1},{-1,-2},{-1,2},{1,-2},{1,2},{2,-1},{2,1}};
                    for(int k = 0; k < 8; k++) {
                        int nx = i + offsets[k][0];
                        int ny = j + offsets[k][1];
                        if(nx == x && ny == y) count++;
                    }
                    break;
                }
                case PIECE_BISHOP:
                case PIECE_ROOK:
                case PIECE_QUEEN: {
                    int dirs[8][2] = {
                        {-1,-1},{-1,1},{1,-1},{1,1},   // diagonales
                        {-1,0},{1,0},{0,-1},{0,1}      // lignes/colonnes
                    };

                    int startDir = 0, endDir = 8;
                    if(cell.piece == PIECE_BISHOP) { startDir = 0; endDir = 4; }    // diagonales
                    else if(cell.piece == PIECE_ROOK) { startDir = 4; endDir = 8; }  // lignes/colonnes
                    // reine = toutes directions (0-7)

                    for(int d = startDir; d < endDir; d++) {
                        int dx = dirs[d][0];
                        int dy = dirs[d][1];
                        int nx = i + dx;
                        int ny = j + dy;
                        while(nx >= 0 && nx < 8 && ny >= 0 && ny < 8) {
                            Cell c = pos->board_model[nx][ny];
                            if(nx == x && ny == y) {
                                count++;
                                break;
                            }
                            if(c.piece != PIECE_NONE) break;
                            nx += dx;
                            ny += dy;
                        }
                    }
                    break;
                }
                case PIECE_KING: {
                    for(int dx = -1; dx <= 1; dx++) {
                        for(int dy = -1; dy <= 1; dy++) {
                            if(dx == 0 && dy == 0) continue;
                            if(i + dx == x && j + dy == y) count++;
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
        && a.toY   == b.toY;
}

void initMoveList(MoveList *movesList) {
    if (!movesList) return;
    movesList->count = 0;
    memset(movesList->moves, 0, sizeof(movesList->moves));
}

void addMoveToList(int fromX, int fromY, int toX, int toY, MoveList *movesList) {
    if (!movesList) return;
    if (movesList->count >= MAX_MOVES) return;

    Move m = {0};
    m.fromX = fromX;
    m.fromY = fromY;
    m.toX = toX;
    m.toY = toY;

    movesList->moves[movesList->count++] = m;
}

void makeMove(Position *pos, Move move, Cell *captured) {
    //utiliser moveTo
    *captured = pos->board_model[move.toX][move.toY];
    pos->board_model[move.toX][move.toY] = pos->board_model[move.fromX][move.fromY];
    pos->board_model[move.fromX][move.fromY] = EMPTY_CELL;
}

void undoMove(Position *pos, Move move, Cell captured) {
    pos->board_model[move.fromX][move.fromY] = pos->board_model[move.toX][move.toY];
    pos->board_model[move.toX][move.toY] = captured;
}

bool isKingInCheck(const Position *pos, Color color) {
    if (!pos) {
        return true;
    }

    int kingX = -1, kingY = -1;
    bool found = false;

    for (int i = 0; i < 8 && !found; i++) {
        for (int j = 0; j < 8; j++) {
            Cell cell = pos->board_model[i][j];
            if (cell.piece == PIECE_KING && cell.color == color) {
                kingX = i;
                kingY = j;
                found = true;
                break; // sort de la boucle interne
            }
        }
    }

    if (kingX == -1 || kingY == -1) return true; // roi absent

    Color enemy = (color == COLOR_WHITE) ? COLOR_BLACK : COLOR_WHITE;
    return countAttacksOnSquare(pos, kingX, kingY, enemy) > 0;
}

bool getKingPosition(const Position *pos, Color color, int *kingX, int *kingY) {
    if (!pos || !kingX || !kingY) {
        return false;
    }

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            Cell cell = pos->board_model[i][j];
            if (cell.piece == PIECE_KING && cell.color == color) {
                *kingX = i;
                *kingY = j;
                return true;
            }
        }
    }

    return false; // roi non trouvé
}

void resetMovesList(MoveList *movesList) {
    movesList->count = 0;
}

void updateAllLegalMoves(Position *pos, Color color, MoveList *movesList) {
    if (!pos || !movesList) return;

    MoveList pseudo;
    initMoveList(&pseudo); // initialisation complète du MoveList temporaire

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            Cell cell = pos->board_model[i][j];
            if (cell.color != color) continue;

            switch(cell.piece) {
                case PIECE_PAWN:   listPawnMoves(pos, i, j, color, &pseudo); break;
                case PIECE_ROOK:   listRookMoves(pos, i, j, color, &pseudo); break;
                case PIECE_BISHOP: listBishopMoves(pos, i, j, color, &pseudo); break;
                case PIECE_KNIGHT: listKnightMoves(pos, i, j, color, &pseudo); break;
                case PIECE_QUEEN:  listQueenMoves(pos, i, j, color, &pseudo); break;
                case PIECE_KING:   listKingMoves(pos, i, j, color, &pseudo); break;
                default: break;
            }
        }
    }

    // Initialiser le MoveList final
    initMoveList(movesList);

    // Filtrer les coups qui laissent le roi en échec
    for (int i = 0; i < pseudo.count; i++) {
        Move m = pseudo.moves[i];
        Cell captured = EMPTY_CELL; // initialisé pour sécurité
        makeMove(pos, m, &captured);

        if (!isKingInCheck(pos, color)) {
            addMoveToList(m.fromX, m.fromY, m.toX, m.toY, movesList);
        }

        undoMove(pos, m, captured);
    }

    if (movesList->count == 0) {
        pos->isFinal = true;
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