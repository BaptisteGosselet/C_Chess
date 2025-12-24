// gui_constants.h

#ifndef GUI_CONSTANTS_H
#define GUI_CONSTANTS_H

// ** WINDOW SETTINGS **
#define WINDOW_TITLE "Chess"
#define BOARD_SIZE 800
#define BOARD_CELL_SIZE (BOARD_SIZE / 8)
#define WINDOW_X_POS SDL_WINDOWPOS_UNDEFINED
#define WINDOW_Y_POS SDL_WINDOWPOS_UNDEFINED
#define WINDOW_WIDTH BOARD_SIZE  //TODO ajouter la zone de boutons
#define WINDOW_HEIGHT BOARD_SIZE //TODO ajouter la zone de boutons

// ** ASSETS SETTINGS **
#define BOARD_PIECES_REPO "assets/img/pieces/"
#define BOARD_PIECES_FILE_FORMAT ".svg"

// ** PIECES NAME **
#define PAWN_WHITE_ASSET_NAME "pawn-w"
#define PAWN_BLACK_ASSET_NAME "pawn-b"
#define KNIGHT_WHITE_ASSET_NAME "knight-w"
#define KNIGHT_BLACK_ASSET_NAME "knight-b"
#define BISHOP_WHITE_ASSET_NAME "bishop-w"
#define BISHOP_BLACK_ASSET_NAME "bishop-b"
#define ROOK_WHITE_ASSET_NAME "rook-w"
#define ROOK_BLACK_ASSET_NAME "rook-b"
#define QUEEN_WHITE_ASSET_NAME "queen-w"
#define QUEEN_BLACK_ASSET_NAME "queen-b"
#define KING_WHITE_ASSET_NAME "king-w"
#define KING_BLACK_ASSET_NAME "king-b"

#endif
