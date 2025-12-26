#ifndef GUI_PIECE_TEXTURE_H
#define GUI_PIECE_TEXTURE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>

extern SDL_Texture *white_pawn_texture;
extern SDL_Texture *white_rook_texture;
extern SDL_Texture *white_knight_texture;
extern SDL_Texture *white_bishop_texture;
extern SDL_Texture *white_queen_texture;
extern SDL_Texture *white_king_texture;
extern SDL_Texture *black_pawn_texture;
extern SDL_Texture *black_rook_texture;
extern SDL_Texture *black_knight_texture;
extern SDL_Texture *black_bishop_texture;
extern SDL_Texture *black_queen_texture;
extern SDL_Texture *black_king_texture;

SDL_Texture* load_texture(SDL_Renderer *renderer, const char* pieceName);
void pieces_textures_init(SDL_Renderer *renderer);
bool getAreTexturesLoaded();

#endif