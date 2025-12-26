#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "gui.h"
#include "gui_constants.h"
#include "gui_board/gui_board.h"
#include "../controller/main_controller.h"
#include "gui_promotion/gui_promotion.h"

SDL_Window* window;
SDL_Renderer* renderer;


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
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void updateRender(void){
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    draw_board(renderer);

    if(awaitPromoteChoice){ 
        int mouse_x, mouse_y;
        SDL_GetMouseState(&mouse_x, &mouse_y);
        draw_promote_choice(renderer, mouse_x, mouse_y);
    }

    SDL_RenderPresent(renderer);
}


void open_window(void) {

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("Erreur SDL_Init: %s", SDL_GetError());
        return;
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
            if (event.type == SDL_QUIT) {  // L'utilisateur ferme la fenêtre
                running = 0;
            }
            handle_board_click(&event); 
            handlePromotePieceClick(&event);
        }
        updateRender();
        SDL_Delay(16); // Petite pause pour ne pas surcharger le CPU (~60 FPS)
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
}