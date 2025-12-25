#include "move.h"
#include "board_model.h"
#include "pieces_type.h"
#include "color.h"
#include <stdbool.h>
#include <stdio.h>

/**
 * Compare deux Move
 */
bool movesEqual(Move a, Move b){
    return a.fromX == b.fromX 
        && a.fromY == b.fromY 
        && a.toX   == b.toX 
        && a.toY   == b.toY 
        && a.promotion == b.promotion;
}

/**
 * Ajoute un mouvement à la liste
 */
void addMoveToList(int fromX, int fromY, int toX, int toY, int promotion, MoveList* movesList){    
    if(movesList->count >= MAX_MOVES) return;

    printf("ADD MOVE : (%d,%d) -> (%d,%d) promo=%d\n", fromX, fromY, toX, toY, promotion);

    movesList->moves[movesList->count++] = (Move){
        .fromX = fromX,
        .fromY = fromY,
        .toX = toX,
        .toY = toY,
        .promotion = promotion
    };
}

/**
 * Réinitialise la liste des mouvements
 */
void resetMovesList(MoveList* movesList){
    for(int i = 0; i < movesList->count; i++){
        movesList->moves[i] = (Move){0,0,0,0,0};
    }
    movesList->count = 0;
}

/**
 * Mouvements coulissants (tour, fou, dame)
 */
void listSlidingMoves(int oI, int oJ, int di, int dj, Color color, MoveList* movesList){
    int i = oI + di;
    int j = oJ + dj;
    while(i >= 0 && i <= 7 && j >= 0 && j <= 7){
        if(board_model[i][j].piece == EMPTY_CELL.piece){
            addMoveToList(oI, oJ, i, j, 0, movesList);
        } else {
            if(board_model[i][j].color != color){
                addMoveToList(oI, oJ, i, j, 0, movesList); // capture
            }
            break;
        }
        i += di; j += dj;
    }
}

/**
 * Liste les mouvements du pion
 */
void listPawnMoves(int oI, int oJ, Color color, MoveList* movesList){
    int dir = (color == COLOR_WHITE) ? -1 : 1;
    int startRow = (color == COLOR_WHITE) ? 6 : 1;
    int nextRow = oI + dir;

    // Avancer d'une case
    if(nextRow >= 0 && nextRow <= 7 && board_model[nextRow][oJ].piece == EMPTY_CELL.piece){
        addMoveToList(oI, oJ, nextRow, oJ, 0, movesList);

        // Avancer de deux cases depuis la position initiale
        int doubleRow = oI + 2*dir;
        if(oI == startRow && board_model[doubleRow][oJ].piece == EMPTY_CELL.piece){
            addMoveToList(oI, oJ, doubleRow, oJ, 0, movesList);
        }
    }

    // Captures diagonales
    for(int dj = -1; dj <= 1; dj += 2){
        int j = oJ + dj;
        if(j >= 0 && j <= 7){
            if(board_model[nextRow][j].piece != EMPTY_CELL.piece && board_model[nextRow][j].color != color){
                addMoveToList(oI, oJ, nextRow, j, 0, movesList);
            }
        }
    }

    // TODO : en passant
    // TODO : promotion
}

/**
 * Liste les mouvements de la tour
 */
void listRookMoves(int oI, int oJ, Color color, MoveList* movesList){
    listSlidingMoves(oI, oJ, -1, 0, color, movesList); // Haut
    listSlidingMoves(oI, oJ, 1, 0, color, movesList);  // Bas
    listSlidingMoves(oI, oJ, 0, -1, color, movesList); // Gauche
    listSlidingMoves(oI, oJ, 0, 1, color, movesList);  // Droite
}

/**
 * Liste les mouvements du fou
 */
void listBishopMoves(int oI, int oJ, Color color, MoveList* movesList){
    listSlidingMoves(oI, oJ, -1, -1, color, movesList); // Haut-Gauche
    listSlidingMoves(oI, oJ, -1, 1, color, movesList);  // Haut-Droite
    listSlidingMoves(oI, oJ, 1, -1, color, movesList);  // Bas-Gauche
    listSlidingMoves(oI, oJ, 1, 1, color, movesList);   // Bas-Droite
}

/**
 * Liste les mouvements de la dame
 */
void listQueenMoves(int oI, int oJ, Color color, MoveList* movesList){
    listRookMoves(oI, oJ, color, movesList);
    listBishopMoves(oI, oJ, color, movesList);
}

/**
 * Liste les mouvements du roi
 */
void listKingMoves(int oI, int oJ, Color color, MoveList* movesList){
    for(int i = oI-1; i <= oI+1; i++){
        for(int j = oJ-1; j <= oJ+1; j++){
            if(i >= 0 && i <= 7 && j >= 0 && j <= 7 && !(i == oI && j == oJ)){
                if(board_model[i][j].piece == EMPTY_CELL.piece || board_model[i][j].color != color){
                    addMoveToList(oI, oJ, i, j, 0, movesList);
                }
            }
        }
    }

    // TODO : roque
    // TODO : gestion des échecs
}

/**
 * Liste les mouvements du cavalier
 */
void listKnightMoves(int oI, int oJ, Color color, MoveList* movesList){
    int moves[8][2] = {
        {-2, -1}, {-2, +1}, {-1, -2}, {-1, +2},
        {+1, -2}, {+1, +2}, {+2, -1}, {+2, +1}
    };

    for(int k = 0; k < 8; k++){
        int i = oI + moves[k][0];
        int j = oJ + moves[k][1];

        if(i >= 0 && i <= 7 && j >= 0 && j <= 7){
            if(board_model[i][j].piece == EMPTY_CELL.piece || board_model[i][j].color != color){
                addMoveToList(oI, oJ, i, j, 0, movesList);
            }
        }
    }
}

/**
 * Met à jour tous les mouvements légaux pour une couleur
 */
void updateAllLegalMoves(Color color, MoveList* movesList){
    printf("LIST MOVES\n");
    resetMovesList(movesList);

    for(int i=0; i<8; i++){
        for(int j=0; j<8; j++){
            if(movesList->count >= MAX_MOVES) return;

            if(board_model[i][j].color == color){
                switch(board_model[i][j].piece){
                    case PIECE_ROOK:   listRookMoves(i, j, color, movesList); break;
                    case PIECE_BISHOP: listBishopMoves(i, j, color, movesList); break;
                    case PIECE_KNIGHT: listKnightMoves(i, j, color, movesList); break;
                    case PIECE_QUEEN:  listQueenMoves(i, j, color, movesList); break;
                    case PIECE_KING:   listKingMoves(i, j, color, movesList); break;
                    case PIECE_PAWN:   listPawnMoves(i, j, color, movesList); break;
                    default: break;
                }
            }
        }
    }
}

/**
 * Vérifie si un mouvement est légal
 */
bool isThisMoveLegal(Move wantedMove, MoveList* movesList){
    for(int i = 0; i < movesList->count; i++){
        if(movesEqual(movesList->moves[i], wantedMove)){
            return true;
        }
    }
    return false;
}
