#ifndef GUI_PROMOTION_H
#define GUI_PROMOTION_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

void draw_promote_choice(SDL_Renderer *renderer, int mouse_x, int mouse_y);
void handlePromotePieceClick(SDL_Event *event);

#endif 
