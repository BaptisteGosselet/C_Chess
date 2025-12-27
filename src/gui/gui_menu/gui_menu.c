#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../gui_constants.h"
#include <SDL2/SDL_ttf.h>
#include "gui_menu_coord.h"

void draw_a_button(SDL_Renderer *renderer, TTF_Font *font, int x, int y, int w, int h, char *text){
    SDL_Rect button_shape = {x, y, w, h};
    SDL_SetRenderDrawColor(renderer, 55, 52, 48, 255);
    SDL_RenderFillRect(renderer, &button_shape);

    SDL_Color textColor = {137, 143, 146, 255};
    SDL_Surface *textSurface = TTF_RenderUTF8_Blended(font, text, textColor);
    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_Rect textRect;
    textRect.w = textSurface->w;
    textRect.h = textSurface->h;
    textRect.x = x + (w - textRect.w) / 2;
    textRect.y = y + (h - textRect.h) / 2;

    SDL_FreeSurface(textSurface);
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
    SDL_DestroyTexture(textTexture);
}

void draw_menu_background(SDL_Renderer *renderer){
    SDL_Rect bg;
    bg.x = MENU_ORIGIN_X;
    bg.y = MENU_ORIGIN_Y;
    bg.w = MENU_WIDTH;   
    bg.h = MENU_HEIGHT;   

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 22, 21, 18, 255);
    SDL_RenderFillRect(renderer, &bg);
}


void draw_players_type(SDL_Renderer *renderer, TTF_Font *font){
    draw_a_button(renderer, font, PLAYER_TYPE_BUTTON_OX, PLAYER_TYPE_BUTTON_OY, PLAYER_TYPE_BUTTON_W, PLAYER_TYPE_BUTTON_H, "J1 : Humain");
    draw_a_button(renderer, font, PLAYER_TYPE_BUTTON_OX, PLAYER_TYPE_BUTTON_OY2, PLAYER_TYPE_BUTTON_W, PLAYER_TYPE_BUTTON_H, "J2 : Humain");
}

void draw_launch_menu(SDL_Renderer *renderer, TTF_Font *font){    
    draw_a_button(
            renderer, 
            font,
            LAUNCH_BUTTON_OX,
            LAUNCH_BUTTON_OY,
            LAUNCH_BUTTON_W,
            LAUNCH_BUTTON_H,
            "Lancer une partie"
        );
}

void draw_menu(SDL_Renderer *renderer, TTF_Font *font){
    draw_menu_background(renderer);
    draw_players_type(renderer, font);
    draw_launch_menu(renderer, font);
}
