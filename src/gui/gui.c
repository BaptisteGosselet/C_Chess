#include <stdio.h>
#include <SDL2/SDL.h>
#include "gui.h"
#include "gui_constants.h"

SDL_Window* window;
SDL_Renderer* renderer;

/**
 * Close the window
 */
void close_window(void){
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}


void draw_board(void){
    
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

    // Afficher le rendu
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

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);

    draw_board();

    // Boucle d'événements
    SDL_Event event;
    int running = 1;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {  // L'utilisateur ferme la fenêtre
                running = 0;
            }
        }
        SDL_Delay(16); // Petite pause pour ne pas surcharger le CPU (~60 FPS)
    }

    close_window();
}

