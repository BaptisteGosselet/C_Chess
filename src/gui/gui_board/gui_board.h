#ifndef GUI_BOARD_H
#define GUI_BOARD_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>

void draw_board(SDL_Renderer *renderer);
void handle_board_click(SDL_Event *event);
void setBoardClick(bool b);
void setMenuMessage(const char *msg);

#endif 
