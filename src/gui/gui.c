#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "gui.h"
#include "gui_constants.h"
#include "gui_board/gui_board.h"

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

    draw_board(renderer);
    SDL_RenderPresent(renderer);


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

