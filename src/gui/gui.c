#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "gui.h"
#include "gui_constants.h"
#include "gui_board/gui_board.h"
#include "../controller/main_controller.h"
#include "gui_promotion/gui_promotion.h"
#include "gui_menu/gui_menu.h"
#include <SDL2/SDL_ttf.h>

SDL_Window* window;
SDL_Renderer* renderer;
TTF_Font* font;

bool awaitPromoteChoice = false; 
typedef struct {
    Position *position;
    int cellI;
    int cellJ;
} PosToPromote;

PosToPromote posToPromoteInstance = {0}; 


/**
 * Close the window
 */
void close_window(void){
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void updateRender(void){
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    
    int mouse_x, mouse_y;
    SDL_GetMouseState(&mouse_x, &mouse_y);

    draw_board(renderer);
    draw_menu(renderer, font, mouse_x, mouse_y);

    if(awaitPromoteChoice){ 
        draw_promote_choice(renderer, mouse_x, mouse_y);
    }

    SDL_RenderPresent(renderer);
}


void open_window(void) {

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("Erreur SDL_Init: %s", SDL_GetError());
        return;
    }

    TTF_Init();
    font = TTF_OpenFont(MENU_FONT_FILE, MENU_FONT_SIZE);
    if (!font) {
        printf("Erreur font: %s\n", TTF_GetError());
    }


    window = SDL_CreateWindow(
        WINDOW_TITLE, 
        WINDOW_X_POS, 
        WINDOW_Y_POS, 
        WINDOW_WIDTH, 
        WINDOW_HEIGHT, 
        SDL_WINDOW_SHOWN
    );
    
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        SDL_Log("Erreur SDL_CreateRenderer: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return;
    }

    updateRender();

    // Boucle d'événements
    SDL_Event event;
    int running = 1;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) { 
                running = 0;
            }
            handle_board_click(&event); 
            handle_menu_click(&event); 
            handlePromotePieceClick(&event);
        }
        updateRender();
        SDL_Delay(16);
    }

    close_window();
}

void awaitPromoteChoiceGUI(Position *pos, int cellI, int cellJ){
    awaitPromoteChoice = true;
    posToPromoteInstance.position = pos;
    posToPromoteInstance.cellI = cellI;
    posToPromoteInstance.cellJ = cellJ;
}

void confirmPromoteChoice(PieceType piece){
    if(!awaitPromoteChoice) return;
    posToPromoteInstance.position->board_model[posToPromoteInstance.cellI][posToPromoteInstance.cellJ].piece = piece;
    awaitPromoteChoice = false;
    posToPromoteInstance.position = NULL;
    posToPromoteInstance.cellI = -1;
    posToPromoteInstance.cellJ = -1;
    letComputerPlayAfterPlayerPromote();
}
