#include "../model/pieces_type.h"
#include "../model/position.h"
#include "../model/game.h"
#include "../model/move.h"
#include "../controller/position_controller/position_controller.h"
#include "../controller/legal_move/legal_move.h"
#include <stdio.h>
#include "position_evaluator/position_evaluator.h"

PieceType aiChoosePieceToPromoteTo(Position *pos){
    if (!pos) return PIECE_QUEEN;
    return PIECE_QUEEN;
}

Move getAnEmptyMove(){
    Move emptyMove;
    emptyMove.fromX = -1;
    emptyMove.fromY = -1;
    emptyMove.toX = -1;
    emptyMove.toY = -1;
    return emptyMove;
}

/** 
 * Return the first move of the legal moves list
 */
Move getFirstLegalMove(Game *game, Color color) {
    MoveList legalMoves; 
    updateAllLegalMoves(&game->position, color, &legalMoves);

    if (legalMoves.count > 0) {
        return legalMoves.moves[0];
    } else {
        return getAnEmptyMove();
    }
}

Move greedyMove(Game *game, Color color) {
    MoveList legalMoves; 
    updateAllLegalMoves(&game->position, color, &legalMoves);
    
    if (legalMoves.count <= 0) {
        return getAnEmptyMove();
    }
    
    Move bestMove = legalMoves.moves[0];  
    int maxScore = -999999; 
    
    for (int i = 0; i < legalMoves.count; i++) {
        Move currentMove = legalMoves.moves[i];
        
        Cell captured;
        makeMove(&game->position, currentMove, &captured);
        
        int score = evalPos(&game->position);
        
        if (color == COLOR_BLACK) {
            score = -score;
        }
        
        undoMove(&game->position, currentMove, captured);
        
        if (score > maxScore) {
            maxScore = score;
            bestMove = currentMove;
        }
    }
    
    return bestMove;
}

void letComputerPlay(Game *game) {
    Color color = game->position.currentColor;
    int playerType = (color == COLOR_WHITE) ? game->whitePlayerType : game->blackPlayerType;

    if (playerType == PLAYER_COMPUTER) {
        Move move = greedyMove(game, color);
        moveTo(game, move.fromX, move.fromY, move.toX, move.toY);
    }
}
