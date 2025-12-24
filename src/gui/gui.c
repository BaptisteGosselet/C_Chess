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

void handle_board_click(SDL_Event *event) {
    if (event->type == SDL_MOUSEBUTTONDOWN) {
        int x = event->button.x;
        int y = event->button.y;

        // Ajuster par rapport à la grille
        int col = (x - BOARD_ORIGIN_X) / BOARD_CELL_SIZE;
        int row = (y - BOARD_ORIGIN_Y) / BOARD_CELL_SIZE;

        if (row >= 0 && row < BOARD_SIZE && col >= 0 && col < BOARD_SIZE) {
            printf("Case cliquée : ligne=%d, colonne=%d\n", row, col);
        }
    }
}

void updateRender(void){
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
    draw_board(renderer);
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
        }
        SDL_Delay(16); // Petite pause pour ne pas surcharger le CPU (~60 FPS)
    }

    close_window();
}

