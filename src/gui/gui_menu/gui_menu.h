#ifndef GUI_MENU_H
#define GUI_MENU_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

void draw_menu(SDL_Renderer *renderer, TTF_Font *font, int mouse_x, int mouse_y);
void handle_menu_click(SDL_Event *event);

#endif 
