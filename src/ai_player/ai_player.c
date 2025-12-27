#include "../model/pieces_type.h"
#include "../model/position.h"
#include "../model/game.h"
#include "../model/move.h"
#include "../controller/position_controller/position_controller.h"
#include "../controller/legal_move/legal_move.h"
#include <stdio.h>

PieceType aiChoosePieceToPromoteTo(Position *pos){
    if (!pos) return PIECE_QUEEN;
    return PIECE_QUEEN;
}

Move getFirstLegalMove(Game *game, Color color) {
    MoveList legalMoves; 
    updateAllLegalMoves(&game->position, color, &legalMoves);

    if (legalMoves.count > 0) {
        return legalMoves.moves[0];
    } else {
        Move emptyMove;
        emptyMove.fromX = -1;
        emptyMove.fromY = -1;
        emptyMove.toX = -1;
        emptyMove.toY = -1;
        return emptyMove;
    }
}

void letComputerPlay(Game *game) {
    Color color = game->position.currentColor;
    int playerType = (color == COLOR_WHITE) ? game->whitePlayerType : game->blackPlayerType;

    if (playerType == PLAYER_COMPUTER) {
        Move move = getFirstLegalMove(game, color);
        moveTo(game, move.fromX, move.fromY, move.toX, move.toY);
    }
}
