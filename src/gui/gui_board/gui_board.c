#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../gui_constants.h"

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

void draw_piece(SDL_Renderer *renderer, SDL_Texture* piece, int x, int y) {
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
    draw_board_cells(renderer);

    SDL_Texture* white_pawn = load_texture(renderer, PAWN_WHITE_ASSET_NAME);
    SDL_Texture* black_queen = load_texture(renderer, QUEEN_BLACK_ASSET_NAME);

    draw_piece(renderer, white_pawn, 0, 6); 
    draw_piece(renderer, black_queen, 4, 0);

}
