#include "../../model/pieces_type.h"
#include "../../model/position.h"
#include "../../model/game.h"
#include "../../model/move.h"
#include "../../controller/position_controller/position_controller.h"
#include "../../controller/legal_move/legal_move.h"
#include <stdio.h>


/**
 * Retourne la valeur d'une pièce
 */
static int getPieceValue(PieceType piece) {
    switch(piece) {
        case PIECE_PAWN:   return 100;
        case PIECE_KNIGHT: return 320;
        case PIECE_BISHOP: return 330;
        case PIECE_ROOK:   return 500;
        case PIECE_QUEEN:  return 900;
        case PIECE_KING:   return 20000;
        default:           return 0;
    }
}

static const int pawnTable[8][8] = {
    { 0,  0,  0,  0,  0,  0,  0,  0},
    {50, 50, 50, 50, 50, 50, 50, 50},
    {10, 10, 20, 30, 30, 20, 10, 10},
    { 5,  5, 10, 25, 25, 10,  5,  5},
    { 0,  0,  0, 20, 20,  0,  0,  0},
    { 5, -5,-10,  0,  0,-10, -5,  5},
    { 5, 10, 10,-20,-20, 10, 10,  5},
    { 0,  0,  0,  0,  0,  0,  0,  0}
};

static const int knightTable[8][8] = {
    {-50,-40,-30,-30,-30,-30,-40,-50},
    {-40,-20,  0,  0,  0,  0,-20,-40},
    {-30,  0, 10, 15, 15, 10,  0,-30},
    {-30,  5, 15, 20, 20, 15,  5,-30},
    {-30,  0, 15, 20, 20, 15,  0,-30},
    {-30,  5, 10, 15, 15, 10,  5,-30},
    {-40,-20,  0,  5,  5,  0,-20,-40},
    {-50,-40,-30,-30,-30,-30,-40,-50}
};

/**
 * Retourne un bonus de position pour une pièce
 */
static int getPositionBonus(PieceType piece, int row, int col, Color color) {
    int adjustedRow = (color == COLOR_WHITE) ? row : (7 - row);
    
    switch(piece) {
        case PIECE_PAWN:
            return pawnTable[adjustedRow][col];
        case PIECE_KNIGHT:
            return knightTable[adjustedRow][col];
        case PIECE_BISHOP:
            // Bonus pour les fous au centre
            if ((row >= 2 && row <= 5) && (col >= 2 && col <= 5)) return 10;
            return 0;
        case PIECE_ROOK:
            // Bonus pour les tours sur les colonnes ouvertes (simplifié)
            return 0;
        case PIECE_QUEEN:
            // La dame préfère le centre en milieu de partie
            if ((row >= 3 && row <= 4) && (col >= 3 && col <= 4)) return 10;
            return 0;
        case PIECE_KING:
            // En début de partie, le roi préfère les coins (sécurité)
            if (adjustedRow >= 6) return 20;
            return 0;
        default:
            return 0;
    }
}

/**
 * Évalue la position du point de vue des BLANCS
 * Positif = avantage blanc, Négatif = avantage noir
 */
int evalPos(Position *position) {
    if (!position) return 0;
    
    int score = 0;
    
    // Parcourir toutes les cases
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            Cell cell = position->board_model[i][j];
            
            if (cell.piece == PIECE_NONE) continue;
            
            int pieceValue = getPieceValue(cell.piece);
            int positionBonus = getPositionBonus(cell.piece, i, j, cell.color);
            int totalValue = pieceValue + positionBonus;
            
            if (cell.color == COLOR_WHITE) {
                score += totalValue;
            } else {
                score -= totalValue;
            }
        }
    }
    
    // Bonus pour la mobilité (nombre de coups légaux)
    MoveList whiteMoves, blackMoves;
    updateAllLegalMoves(position, COLOR_WHITE, &whiteMoves);
    updateAllLegalMoves(position, COLOR_BLACK, &blackMoves);
    
    score += (whiteMoves.count - blackMoves.count) * 5;
    
    // Pénalité si le roi est en échec
    if (isKingInCheck(position, COLOR_WHITE)) {
        score -= 50;
    }
    if (isKingInCheck(position, COLOR_BLACK)) {
        score += 50;
    }
    
    return score;
}
