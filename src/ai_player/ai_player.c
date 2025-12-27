#include "../model/pieces_type.h"
#include "../model/position.h"
#include "../model/game.h"
#include "../model/move.h"
#include "../controller/position_controller/position_controller.h"
#include "../controller/legal_move/legal_move.h"
#include <stdio.h>
#include "position_evaluator/position_evaluator.h"
#include "ai_player_config.h"

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

/**
 * Joue le coup qui amène le plus grand score depuis une position locale
 */
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

static int minmax(Position *position, int depth, int alpha, int beta, bool maximizingPlayer) {
    if (depth == 0 || position->isFinal) {
        return evalPos(position);
    }
    
    Color currentColor = maximizingPlayer ? COLOR_WHITE : COLOR_BLACK;
    MoveList legalMoves;
    updateAllLegalMoves(position, currentColor, &legalMoves);
    
    if (legalMoves.count == 0) {
        if (isKingInCheck(position, currentColor)) {
            return maximizingPlayer ? (-MATE_SCORE + depth) : (MATE_SCORE - depth);
        } else {
            return 0;
        }
    }
    
    if (maximizingPlayer) {
        int maxEval = MIN_SCORE;
        
        for (int i = 0; i < legalMoves.count; i++) {
            Cell captured;
            makeMove(position, legalMoves.moves[i], &captured);
            
            int eval = minmax(position, depth - 1, alpha, beta, false);
            
            undoMove(position, legalMoves.moves[i], captured);
            
            maxEval = (eval > maxEval) ? eval : maxEval;
            alpha = (alpha > eval) ? alpha : eval;
            
            if (beta <= alpha) {
                break;
            }
        }
        return maxEval;
        
    } else {
        int minEval = MAX_SCORE;
        
        for (int i = 0; i < legalMoves.count; i++) {
            Cell captured;
            makeMove(position, legalMoves.moves[i], &captured);
            
            int eval = minmax(position, depth - 1, alpha, beta, true);
            
            undoMove(position, legalMoves.moves[i], captured);
            
            minEval = (eval < minEval) ? eval : minEval;
            beta = (beta < eval) ? beta : eval;
            
            if (beta <= alpha) {
                break;
            }
        }
        return minEval;
    }
}

Move minmaxMove(Game *game, Color color, int depth) {
    MoveList legalMoves;
    updateAllLegalMoves(&game->position, color, &legalMoves);
    
    if (legalMoves.count <= 0) {
        return getAnEmptyMove();
    }
    
    Move bestMove = legalMoves.moves[0];
    int bestScore = (color == COLOR_WHITE) ? MIN_SCORE : MAX_SCORE;
    bool maximizingPlayer = (color == COLOR_WHITE);
        
    for (int i = 0; i < legalMoves.count; i++) {
        Cell captured;
        makeMove(&game->position, legalMoves.moves[i], &captured);
        
        int score = minmax(&game->position, depth - 1, MIN_SCORE, MAX_SCORE, !maximizingPlayer);
        
        undoMove(&game->position, legalMoves.moves[i], captured);
        
        if (maximizingPlayer) {
            if (score > bestScore) {
                bestScore = score;
                bestMove = legalMoves.moves[i];
            }
        } else {
            if (score < bestScore) {
                bestScore = score;
                bestMove = legalMoves.moves[i];
            }
        }
    }

    return bestMove;
}

void letComputerPlay(Game *game) {
    Color color = game->position.currentColor;
    int playerType = (color == COLOR_WHITE) ? game->whitePlayerType : game->blackPlayerType;

    Move move = getAnEmptyMove();
    if (playerType == PLAYER_COMPUTER) {
        if(COMPUTER_MODE == COMPUTER_GREEDY){
            move = greedyMove(game, color);
        }
        else if(COMPUTER_MODE == COMPUTER_MINMAX){
            move = minmaxMove(game, color, MINMAX_DEPTH);
        }
        else {
            move = getFirstLegalMove(game, color);
        }
        moveTo(game, move.fromX, move.fromY, move.toX, move.toY);
    }
}
