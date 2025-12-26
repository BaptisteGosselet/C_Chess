

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include "../gui_constants.h"

bool areTexturesLoaded = false;

// Pieces texture
// TODO : getters 
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

//TODO: suppr après la mise des getters
bool getAreTexturesLoaded(){
    return areTexturesLoaded;
}

void loadTextureFirst(SDL_Renderer *renderer){
    if(!areTexturesLoaded){
        pieces_textures_init(renderer);
    }
}