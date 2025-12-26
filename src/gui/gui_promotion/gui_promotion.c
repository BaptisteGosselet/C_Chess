#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../gui_constants.h"
#include "../pieces_texture/pieces_texture.h"
#include <stdio.h>
#include "../../model/pieces_type.h"
#include "../gui.h"

int get_hovered_piece(int mouse_x, int mouse_y) {
    // Vérifie sur quelle "case" de promotion la souris se trouve
    int start_x = BOARD_SIZE / 4;
    int y = BOARD_ORIGIN_Y + (BOARD_SIZE / 2) - BOARD_CELL_SIZE;

    for (int i = 0; i < 4; i++) {
        SDL_Rect rect;
        rect.x = start_x + (BOARD_CELL_SIZE * i);
        rect.y = y;
        rect.w = BOARD_CELL_SIZE;
        rect.h = BOARD_CELL_SIZE;

        if (mouse_x >= rect.x && mouse_x <= rect.x + rect.w &&
            mouse_y >= rect.y && mouse_y <= rect.y + rect.h) {
            return i; // la pièce survolée
        }
    }
    return -1; // aucune
}

void draw_a_piece_on_promotion(SDL_Renderer *renderer, SDL_Texture* piece, int place, int hovered) {
    SDL_Rect rect_fill;
    rect_fill.x = (BOARD_SIZE/4) + (BOARD_CELL_SIZE * place);
    rect_fill.y = BOARD_ORIGIN_Y + (BOARD_SIZE / 2) - BOARD_CELL_SIZE;
    rect_fill.w = BOARD_CELL_SIZE;   
    rect_fill.h = BOARD_CELL_SIZE;   

    SDL_Rect rect_piece = rect_fill;

    if (hovered) {
        SDL_SetRenderDrawColor(renderer, 200, 200, 50, 255); // couleur de survol
    } else {
        SDL_SetRenderDrawColor(renderer, 147, 147, 147, 255); // couleur normale
    }
    SDL_RenderFillRect(renderer, &rect_fill);
    
    SDL_RenderCopy(renderer, piece, NULL, &rect_piece);
}

void draw_promote_choice(SDL_Renderer *renderer, int mouse_x, int mouse_y){
    SDL_Texture* choices[4];

    if (1){ //TODO: color
        choices[0] = white_queen_texture;
        choices[1] = white_rook_texture;
        choices[2] = white_bishop_texture;
        choices[3] = white_knight_texture;
    }
    else{
        choices[0] = black_queen_texture;
        choices[1] = black_rook_texture;
        choices[2] = black_bishop_texture;
        choices[3] = black_knight_texture;
    }

    int hovered = get_hovered_piece(mouse_x, mouse_y);

    for (int i = 0; i < 4; i++) {
        draw_a_piece_on_promotion(renderer, choices[i], i, (i == hovered));
    }
}

void handlePromotePieceClick(SDL_Event *event){
    PieceType pieces[4] = {PIECE_QUEEN, PIECE_ROOK, PIECE_BISHOP, PIECE_KNIGHT};
    
    if (event->type == SDL_MOUSEBUTTONDOWN) {
            
        int x = event->button.x;
        int y = event->button.y;
        int hovered = get_hovered_piece(x, y);
            
        if (hovered != -1) {
            confirmPromoteChoice(pieces[hovered]);
        }
    }

}