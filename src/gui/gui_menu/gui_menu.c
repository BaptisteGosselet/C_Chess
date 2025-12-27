#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../gui_constants.h"
#include <SDL2/SDL_ttf.h>
#include "gui_menu_coord.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "../../controller/menu_params_controller/menu_params_controller.h"
#include "../../controller/main_controller.h"
#include "../../model/player_type.h"

static char menuMessage[256] = "Bienvenue !";
static int aiDifficulty = 3;
#define MAX_AI_DIFFICULTY 4 

void setMenuMessage(const char *msg) {
    if (msg) {
        strncpy(menuMessage, msg, sizeof(menuMessage) - 1);
        menuMessage[sizeof(menuMessage) - 1] = '\0';
    }
}

int getAiDifficulty() {
    return aiDifficulty;
}

void cycleAiDifficulty() {
    aiDifficulty++;
    if (aiDifficulty > MAX_AI_DIFFICULTY) {
        aiDifficulty = 1;
    }
}

bool is_mouse_over_button(int mouse_x, int mouse_y, int x, int y, int w, int h){
    return (mouse_x >= x && mouse_x <= x + w && mouse_y >= y && mouse_y <= y + h);
}

void draw_a_button(SDL_Renderer *renderer, TTF_Font *font, int x, int y, int w, int h, char *text, bool is_hovered){
    SDL_Rect button_shape = {x, y, w, h};
    SDL_Color textColor;
    
    if(is_hovered){
        SDL_SetRenderDrawColor(renderer, 75, 72, 68, 255); 
        textColor = (SDL_Color){198, 77, 6, 255}; 
    } else {
        SDL_SetRenderDrawColor(renderer, 55, 52, 48, 255);
        textColor = (SDL_Color){137, 143, 146, 255};
    }
    
    SDL_RenderFillRect(renderer, &button_shape);
    
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

void draw_menu_message(SDL_Renderer *renderer, TTF_Font *font) {
    SDL_Color textColor = {198, 77, 6, 255};
    
    SDL_Surface *textSurface = TTF_RenderUTF8_Blended(font, menuMessage, textColor);
    if (!textSurface) return;
    
    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_Rect textRect;
    textRect.w = textSurface->w;
    textRect.h = textSurface->h;
    textRect.x = MESSAGE_OX + (MESSAGE_W - textRect.w) / 2;
    textRect.y = MESSAGE_OY + (MESSAGE_H - textRect.h) / 2;
    
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

void draw_difficulty_button(SDL_Renderer *renderer, TTF_Font *font, int mouse_x, int mouse_y) {
    bool hover = is_mouse_over_button(mouse_x, mouse_y, DIFFICULTY_BUTTON_OX, DIFFICULTY_BUTTON_OY, 
                                      DIFFICULTY_BUTTON_W, DIFFICULTY_BUTTON_H);
    
    char difficultyText[64];
    snprintf(difficultyText, sizeof(difficultyText), "Niveau de l'ordi : %d", aiDifficulty);
    
    draw_a_button(renderer, font, DIFFICULTY_BUTTON_OX, DIFFICULTY_BUTTON_OY, 
                  DIFFICULTY_BUTTON_W, DIFFICULTY_BUTTON_H, difficultyText, hover);
}

void draw_undo_button(SDL_Renderer *renderer, TTF_Font *font, int mouse_x, int mouse_y) {
    bool hover = is_mouse_over_button(mouse_x, mouse_y, UNDO_BUTTON_OX, UNDO_BUTTON_OY, 
                                      UNDO_BUTTON_W, UNDO_BUTTON_H);
    
    draw_a_button(renderer, font, UNDO_BUTTON_OX, UNDO_BUTTON_OY, 
                  UNDO_BUTTON_W, UNDO_BUTTON_H, "Reprise de coup", hover);
}

void draw_players_type(SDL_Renderer *renderer, TTF_Font *font, int mouse_x, int mouse_y){
    bool hover1 = is_mouse_over_button(mouse_x, mouse_y, PLAYER_TYPE_BUTTON_OX, PLAYER_TYPE_BUTTON_OY, 
                                       PLAYER_TYPE_BUTTON_W, PLAYER_TYPE_BUTTON_H);
    bool hover2 = is_mouse_over_button(mouse_x, mouse_y, PLAYER_TYPE_BUTTON_OX, PLAYER_TYPE_BUTTON_OY2, 
                                       PLAYER_TYPE_BUTTON_W, PLAYER_TYPE_BUTTON_H);
        
    char *j1Text = (paramWhitePlayerType == PLAYER_HUMAN) ? "J1 : Humain" : "J1 : Ordi";
    char *j2Text = (paramBlackPlayerType == PLAYER_HUMAN) ? "J2 : Humain" : "J2 : Ordi";

    draw_a_button(renderer, font, PLAYER_TYPE_BUTTON_OX, PLAYER_TYPE_BUTTON_OY, 
                  PLAYER_TYPE_BUTTON_W, PLAYER_TYPE_BUTTON_H, j1Text, hover1);
    draw_a_button(renderer, font, PLAYER_TYPE_BUTTON_OX, PLAYER_TYPE_BUTTON_OY2, 
                  PLAYER_TYPE_BUTTON_W, PLAYER_TYPE_BUTTON_H, j2Text, hover2);
}

void draw_launch_menu(SDL_Renderer *renderer, TTF_Font *font, int mouse_x, int mouse_y){
    bool hover = is_mouse_over_button(mouse_x, mouse_y, LAUNCH_BUTTON_OX, LAUNCH_BUTTON_OY, 
                                      LAUNCH_BUTTON_W, LAUNCH_BUTTON_H);
    
    draw_a_button(
            renderer, 
            font,
            LAUNCH_BUTTON_OX,
            LAUNCH_BUTTON_OY,
            LAUNCH_BUTTON_W,
            LAUNCH_BUTTON_H,
            "Lancer une partie",
            hover
        );
}

void draw_menu(SDL_Renderer *renderer, TTF_Font *font, int mouse_x, int mouse_y){
    draw_menu_background(renderer);
    draw_menu_message(renderer, font);
    draw_difficulty_button(renderer, font, mouse_x, mouse_y);
    draw_undo_button(renderer, font, mouse_x, mouse_y);
    draw_players_type(renderer, font, mouse_x, mouse_y);
    draw_launch_menu(renderer, font, mouse_x, mouse_y);
}

void handle_menu_click(SDL_Event *event){
    if(event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT){
        int mouse_x = event->button.x;
        int mouse_y = event->button.y;
        
        if(is_mouse_over_button(mouse_x, mouse_y, DIFFICULTY_BUTTON_OX, DIFFICULTY_BUTTON_OY, 
                                DIFFICULTY_BUTTON_W, DIFFICULTY_BUTTON_H)){
            cycleAiDifficulty();
        }
        
        if(is_mouse_over_button(mouse_x, mouse_y, UNDO_BUTTON_OX, UNDO_BUTTON_OY, 
                                UNDO_BUTTON_W, UNDO_BUTTON_H)){
            undoLastMove();
        }
        
        if(is_mouse_over_button(mouse_x, mouse_y, PLAYER_TYPE_BUTTON_OX, PLAYER_TYPE_BUTTON_OY, 
                                PLAYER_TYPE_BUTTON_W, PLAYER_TYPE_BUTTON_H)){
            changeWhitePlayerType();
        }
        
        if(is_mouse_over_button(mouse_x, mouse_y, PLAYER_TYPE_BUTTON_OX, PLAYER_TYPE_BUTTON_OY2, 
                                PLAYER_TYPE_BUTTON_W, PLAYER_TYPE_BUTTON_H)){
            changeBlackPlayerType();
        }
        
        if(is_mouse_over_button(mouse_x, mouse_y, LAUNCH_BUTTON_OX, LAUNCH_BUTTON_OY, 
                                LAUNCH_BUTTON_W, LAUNCH_BUTTON_H)){
            init_params_game();
        }
    }
}