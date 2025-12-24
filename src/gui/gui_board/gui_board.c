#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../gui_constants.h"
#include <stdbool.h>

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


void draw_a_piece(SDL_Renderer *renderer, SDL_Texture* piece, int x, int y) {
    SDL_Rect dest;
    dest.x = x * BOARD_CELL_SIZE;
    dest.y = y * BOARD_CELL_SIZE;
    dest.w = BOARD_CELL_SIZE;
    dest.h = BOARD_CELL_SIZE;
    SDL_RenderCopy(renderer, piece, NULL, &dest);
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
            rect.x = i*BOARD_CELL_SIZE;   
            rect.y = j*BOARD_CELL_SIZE;  
            rect.w = BOARD_CELL_SIZE;   
            rect.h = BOARD_CELL_SIZE;   

            SDL_RenderFillRect(renderer, &rect);
        }
    }

}

void draw_board(SDL_Renderer *renderer){
    if(!areTexturesLoaded){
        pieces_textures_init(renderer);
    }
    draw_board_cells(renderer);


    //tmp drawing
    for(int i=0; i<8;i++){
            draw_a_piece(renderer, black_pawn_texture, i, 1);
            draw_a_piece(renderer, white_pawn_texture, i, 6);
    }
    draw_a_piece(renderer, black_king_texture, 4, 0);
    draw_a_piece(renderer, black_queen_texture, 3, 0);
    draw_a_piece(renderer, white_king_texture, 4, 7);
    draw_a_piece(renderer, white_queen_texture, 3, 7);
    draw_a_piece(renderer, black_rook_texture, 0, 0);
    draw_a_piece(renderer, white_rook_texture, 0, 7);
    draw_a_piece(renderer, black_rook_texture, 7, 0);
    draw_a_piece(renderer, white_rook_texture, 7, 7);
    draw_a_piece(renderer, black_knight_texture, 1, 0);
    draw_a_piece(renderer, black_knight_texture, 6, 0);
    draw_a_piece(renderer, white_knight_texture, 6, 7);
    draw_a_piece(renderer, white_knight_texture, 1, 7);
    draw_a_piece(renderer, black_bishop_texture, 2, 0);
    draw_a_piece(renderer, black_bishop_texture, 5, 0);
    draw_a_piece(renderer, white_bishop_texture, 5, 7);
    draw_a_piece(renderer, white_bishop_texture, 2, 7);

}
