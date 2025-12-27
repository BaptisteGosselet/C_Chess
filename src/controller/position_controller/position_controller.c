#include "../../model/position.h"
#include "../../model/game.h"
#include "../legal_move/legal_move.h"
#include <stdio.h>
#include "../../gui/gui.h"
#include "../../model/player_type.h"
#include "../../ai_player/ai_player.h"

void init_board(Position *pos) {
    pos->board_model[0][0] = (Cell){COLOR_BLACK, PIECE_ROOK};
    pos->board_model[0][1] = (Cell){COLOR_BLACK, PIECE_KNIGHT};
    pos->board_model[0][2] = (Cell){COLOR_BLACK, PIECE_BISHOP};
    pos->board_model[0][3] = (Cell){COLOR_BLACK, PIECE_QUEEN};
    pos->board_model[0][4] = (Cell){COLOR_BLACK, PIECE_KING};
    pos->board_model[0][5] = (Cell){COLOR_BLACK, PIECE_BISHOP};
    pos->board_model[0][6] = (Cell){COLOR_BLACK, PIECE_KNIGHT};
    pos->board_model[0][7] = (Cell){COLOR_BLACK, PIECE_ROOK};

    for (int i = 0; i < 8; i++)
        pos->board_model[1][i] = (Cell){COLOR_BLACK, PIECE_PAWN};

    for (int i = 2; i < 6; i++)
        for (int j = 0; j < 8; j++)
            pos->board_model[i][j] = EMPTY_CELL;

    for (int i = 0; i < 8; i++)
        pos->board_model[6][i] = (Cell){COLOR_WHITE, PIECE_PAWN};

    pos->board_model[7][0] = (Cell){COLOR_WHITE, PIECE_ROOK};
    pos->board_model[7][1] = (Cell){COLOR_WHITE, PIECE_KNIGHT};
    pos->board_model[7][2] = (Cell){COLOR_WHITE, PIECE_BISHOP};
    pos->board_model[7][3] = (Cell){COLOR_WHITE, PIECE_QUEEN};
    pos->board_model[7][4] = (Cell){COLOR_WHITE, PIECE_KING};
    pos->board_model[7][5] = (Cell){COLOR_WHITE, PIECE_BISHOP};
    pos->board_model[7][6] = (Cell){COLOR_WHITE, PIECE_KNIGHT};
    pos->board_model[7][7] = (Cell){COLOR_WHITE, PIECE_ROOK};
}

void updateLegalMoves(Position *pos) {
    updateAllLegalMoves(
        pos,
        pos->currentColor,
        &pos->currentLegalMovesList
    );
}

void init_position(Position *pos) {
    pos->currentColor = COLOR_WHITE;

    pos->whiteCanKingCastle  = true;
    pos->whiteCanQueenCastle = true;
    pos->blackCanKingCastle  = true;
    pos->blackCanQueenCastle = true;

    pos->isFinal = false;
    pos->whitePushedPawn = -1;
    pos->blackPushedPawn = -1;

    init_board(pos);
    updateLegalMoves(pos);
}

void changeColorTurn(Position *pos) {
    pos->currentColor =
        (pos->currentColor == COLOR_WHITE) ? COLOR_BLACK : COLOR_WHITE;
}

static void checkMoveIndicator(Position *pos, int oI, int oJ, int dI) {
    pos->whitePushedPawn = -1;
    pos->blackPushedPawn = -1;

    Cell moved = pos->board_model[oI][oJ];

    if (moved.piece == PIECE_KING) {
        if (moved.color == COLOR_WHITE)
            pos->whiteCanKingCastle = pos->whiteCanQueenCastle = false;
        else
            pos->blackCanKingCastle = pos->blackCanQueenCastle = false;
    }
    else if (moved.piece == PIECE_ROOK) {
        if (oJ == 0) {
            if (moved.color == COLOR_WHITE)
                pos->whiteCanQueenCastle = false;
            else
                pos->blackCanQueenCastle = false;
        }
        else if (oJ == 7) {
            if (moved.color == COLOR_WHITE)
                pos->whiteCanKingCastle = false;
            else
                pos->blackCanKingCastle = false;
        }
    }
    else if (moved.piece == PIECE_PAWN) {
        if (moved.color == COLOR_WHITE && oI == 6 && dI == 4)
            pos->whitePushedPawn = oJ;
        else if (moved.color == COLOR_BLACK && oI == 1 && dI == 3)
            pos->blackPushedPawn = oJ;
    }
}

static void handleCastleRook(Position *pos, int oY, int dX, int dY) {
    if (pos->board_model[dX][dY].piece != PIECE_KING)
        return;

    if (dY - oY == 2) {
        pos->board_model[dX][dY - 1] = pos->board_model[dX][dY + 1];
        pos->board_model[dX][dY + 1] = EMPTY_CELL;
    }
    else if (oY - dY == 2) {
        pos->board_model[dX][dY + 1] = pos->board_model[dX][dY - 2];
        pos->board_model[dX][dY - 2] = EMPTY_CELL;
    }
}

static void handleEnPassant(Position *pos, int oJ, int dI, int dJ) {
    Cell moved = pos->board_model[dI][dJ];

    if (moved.piece != PIECE_PAWN || oJ == dJ)
        return;

    if (moved.color == COLOR_WHITE) {
        if (pos->board_model[dI + 1][dJ].piece == PIECE_PAWN &&
            pos->board_model[dI + 1][dJ].color == COLOR_BLACK) {
            pos->board_model[dI + 1][dJ] = EMPTY_CELL;
        }
    } else {
        if (pos->board_model[dI - 1][dJ].piece == PIECE_PAWN &&
            pos->board_model[dI - 1][dJ].color == COLOR_WHITE) {
            pos->board_model[dI - 1][dJ] = EMPTY_CELL;
        }
    }
}

/**
 * return true if the promotion has to be handled by an human
 */
static bool handlePromotion(Game *game, int row, int col) {
    Cell moved = game->position.board_model[row][col];
    
    if (moved.piece != PIECE_PAWN)
        return false;

    if (!((moved.color == COLOR_WHITE && row == 0) ||
        (moved.color == COLOR_BLACK && row == 7)))
        return false;


    if ((moved.color == COLOR_WHITE && game->whitePlayerType == PLAYER_HUMAN) ||
        (moved.color == COLOR_BLACK && game->blackPlayerType == PLAYER_HUMAN)) {
        awaitPromoteChoiceGUI(&game->position, row, col);
        return true;
    }
    else {
        game->position.board_model[row][col].piece = aiChoosePieceToPromoteTo(&game->position);
        return false;
    }
}

void moveTo(Game *game, int oX, int oY, int dX, int dY) {
    Position *pos = &game->position;

    if (oX < 0 || oX > 7 || oY < 0 || oY > 7 ||
        dX < 0 || dX > 7 || dY < 0 || dY > 7)
        return;

    checkMoveIndicator(pos, oX, oY, dX);

    pos->board_model[dX][dY] = pos->board_model[oX][oY];
    pos->board_model[oX][oY] = EMPTY_CELL;
    
    
    
    handleCastleRook(pos, oY, dX, dY);
    handleEnPassant(pos, oY, dX, dY);
    
    bool hasToBePromotedByAnHuman = handlePromotion(game, dX, dY);

    changeColorTurn(pos);
    updateLegalMoves(pos);

    if (pos->isFinal) {
        if (isKingInCheck(pos, pos->currentColor)) {
            printf(pos->currentColor == COLOR_WHITE
                   ? "CHECKMATE : black wins\n"
                   : "CHECKMATE : white wins\n");
        } else {
            printf("DRAW\n");
        }
    }
    else if(!hasToBePromotedByAnHuman){
        letComputerPlay(game);
    }
}