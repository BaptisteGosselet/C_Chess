#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../gui_constants.h"
#include <stdbool.h>
#include "../../model/position.h"
#include "../../model/pieces_type.h"
#include "../../controller/main_controller.h"
#include "../../controller/legal_move/legal_move.h"

bool areTexturesLoaded = false;

// Pieces texture
SDL_Texture *white_pawn_texture   = NULL;
SDL_Texture *white_rook_texture   = NULL;
SDL_Texture *white_knight_texture = NULL;
SDL_Texture *white_bishop_texture = NULL;
SDL_Texture *white_queen_texture  = NULL;
SDL_Texture *white_king_texture   = NULL;
SDL_Texture *black_pawn_texture   = NULL;
SDL_Texture *black_rook_texture   = NULL;
SDL_Texture *black_knight_texture = NULL;
SDL_Texture *black_bishop_texture = NULL;
SDL_Texture *black_queen_texture  = NULL;
SDL_Texture *black_king_texture   = NULL;

/**
 * load piece image by filename
 * TODO : une fonction par pièce ?
 */
SDL_Texture* load_texture(SDL_Renderer *renderer, const char* pieceName) {
    char file[256]; 
    snprintf(file, sizeof(file), "%s%s%s", BOARD_PIECES_REPO, pieceName, BOARD_PIECES_FILE_FORMAT);
    SDL_Surface* surface = IMG_Load(file);
    if (!surface) {
        SDL_Log("IMG_Load error: %s", IMG_GetError());
        return NULL;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}


void pieces_textures_init(SDL_Renderer *renderer) {     
    white_pawn_texture   = load_texture(renderer, PAWN_WHITE_ASSET_NAME);
    white_rook_texture   = load_texture(renderer, ROOK_WHITE_ASSET_NAME);
    white_knight_texture = load_texture(renderer, KNIGHT_WHITE_ASSET_NAME);
    white_bishop_texture = load_texture(renderer, BISHOP_WHITE_ASSET_NAME);
    white_queen_texture  = load_texture(renderer, QUEEN_WHITE_ASSET_NAME);
    white_king_texture   = load_texture(renderer, KING_WHITE_ASSET_NAME);
    black_pawn_texture   = load_texture(renderer, PAWN_BLACK_ASSET_NAME);
    black_rook_texture   = load_texture(renderer, ROOK_BLACK_ASSET_NAME);
    black_knight_texture = load_texture(renderer, KNIGHT_BLACK_ASSET_NAME);
    black_bishop_texture = load_texture(renderer, BISHOP_BLACK_ASSET_NAME);
    black_queen_texture  = load_texture(renderer, QUEEN_BLACK_ASSET_NAME);
    black_king_texture   = load_texture(renderer, KING_BLACK_ASSET_NAME);
    areTexturesLoaded = true;
}


void draw_a_piece(SDL_Renderer *renderer, SDL_Texture* piece, int ligne, int col) {
    SDL_Rect dest;
    dest.x = BOARD_ORIGIN_X + col * BOARD_CELL_SIZE;
    dest.y = BOARD_ORIGIN_Y + ligne * BOARD_CELL_SIZE;
    dest.w = BOARD_CELL_SIZE;
    dest.h = BOARD_CELL_SIZE;
    SDL_RenderCopy(renderer, piece, NULL, &dest);
}

void draw_highlight_cell(SDL_Renderer *renderer){
    const int *selected = controllerGetSelectedCell();
    if (selected[0] != -1 && selected[1] != -1) {
        SDL_SetRenderDrawColor(renderer, 130, 151, 105, 255);
        SDL_Rect rect;
        rect.x = BOARD_ORIGIN_X + selected[1]*BOARD_CELL_SIZE;   
        rect.y = BOARD_ORIGIN_Y + selected[0]*BOARD_CELL_SIZE;  
        rect.w = BOARD_CELL_SIZE;   
        rect.h = BOARD_CELL_SIZE;   
        SDL_RenderFillRect(renderer, &rect);
    }
}

void draw_king_check(SDL_Renderer *renderer){
    const Position *pos = controllerGetPosition();
    int kingX = -1, kingY = -1;
    getKingPosition(pos, pos->currentColor, &kingX, &kingY);
    if (isKingInCheck(pos, pos->currentColor)){
        SDL_SetRenderDrawColor(renderer, 214, 54, 39, 255);
        SDL_Rect rect;
        rect.x = BOARD_ORIGIN_X + kingY*BOARD_CELL_SIZE;   
        rect.y = BOARD_ORIGIN_Y + kingX*BOARD_CELL_SIZE;  
        rect.w = BOARD_CELL_SIZE;   
        rect.h = BOARD_CELL_SIZE;   
        SDL_RenderFillRect(renderer, &rect);
    }
}

void draw_board_cells(SDL_Renderer *renderer){
    
    for(int i=0; i<8; i++){
        for(int j=0; j<8; j++){

            if((i%2) == (j%2)){
                SDL_SetRenderDrawColor(renderer, 239, 216, 180, 255);
            }
            else{
                SDL_SetRenderDrawColor(renderer, 180, 135, 98, 255);
            }

            SDL_Rect rect;
            rect.x = BOARD_ORIGIN_X + i*BOARD_CELL_SIZE;   
            rect.y = BOARD_ORIGIN_Y + j*BOARD_CELL_SIZE;  
            rect.w = BOARD_CELL_SIZE;   
            rect.h = BOARD_CELL_SIZE;   

            SDL_RenderFillRect(renderer, &rect);
        }
    }
}

void draw_board_pieces_from_model(SDL_Renderer *renderer) {
    const Position *pos = controllerGetPosition();

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {

            Cell cell = pos->board_model[i][j];

            if (cell.color == COLOR_WHITE) {
                if (cell.piece == PIECE_PAWN)
                    draw_a_piece(renderer, white_pawn_texture, i, j);
                else if (cell.piece == PIECE_ROOK)
                    draw_a_piece(renderer, white_rook_texture, i, j);
                else if (cell.piece == PIECE_KNIGHT)
                    draw_a_piece(renderer, white_knight_texture, i, j);
                else if (cell.piece == PIECE_BISHOP)
                    draw_a_piece(renderer, white_bishop_texture, i, j);
                else if (cell.piece == PIECE_QUEEN)
                    draw_a_piece(renderer, white_queen_texture, i, j);
                else if (cell.piece == PIECE_KING)
                    draw_a_piece(renderer, white_king_texture, i, j);
            }
            else if (cell.color == COLOR_BLACK) {
                if (cell.piece == PIECE_PAWN)
                    draw_a_piece(renderer, black_pawn_texture, i, j);
                else if (cell.piece == PIECE_ROOK)
                    draw_a_piece(renderer, black_rook_texture, i, j);
                else if (cell.piece == PIECE_KNIGHT)
                    draw_a_piece(renderer, black_knight_texture, i, j);
                else if (cell.piece == PIECE_BISHOP)
                    draw_a_piece(renderer, black_bishop_texture, i, j);
                else if (cell.piece == PIECE_QUEEN)
                    draw_a_piece(renderer, black_queen_texture, i, j);
                else if (cell.piece == PIECE_KING)
                    draw_a_piece(renderer, black_king_texture, i, j);
            }
        }
    }
}


void draw_board(SDL_Renderer *renderer) {
    if (!areTexturesLoaded) {
        pieces_textures_init(renderer);
    }
    draw_board_cells(renderer);
    draw_highlight_cell(renderer);
    draw_king_check(renderer);
    draw_board_pieces_from_model(renderer);
}

