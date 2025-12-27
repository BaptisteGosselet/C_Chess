#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../gui_constants.h"
#include <stdbool.h>
#include "../../model/position.h"
#include "../../model/pieces_type.h"
#include "../../controller/main_controller.h"
#include "../../controller/legal_move/legal_move.h"
#include "../pieces_texture/pieces_texture.h"

bool boardClickEnabled = true;


void draw_a_piece_on_the_board(SDL_Renderer *renderer, SDL_Texture* piece, int ligne, int col) {
    SDL_Rect dest;
    dest.x = BOARD_ORIGIN_X + col * BOARD_CELL_SIZE;
    dest.y = BOARD_ORIGIN_Y + ligne * BOARD_CELL_SIZE;
    dest.w = BOARD_CELL_SIZE;
    dest.h = BOARD_CELL_SIZE;
    SDL_RenderCopy(renderer, piece, NULL, &dest);
}

void draw_highlight_cell(SDL_Renderer *renderer){
    const int *selected = controllerGetSelectedCell();
    if (selected[0] != -1 && selected[1] != -1) {
        SDL_SetRenderDrawColor(renderer, 130, 151, 105, 255);
        SDL_Rect rect;
        rect.x = BOARD_ORIGIN_X + selected[1]*BOARD_CELL_SIZE;   
        rect.y = BOARD_ORIGIN_Y + selected[0]*BOARD_CELL_SIZE;  
        rect.w = BOARD_CELL_SIZE;   
        rect.h = BOARD_CELL_SIZE;   
        SDL_RenderFillRect(renderer, &rect);
    }
}

void draw_king_check(SDL_Renderer *renderer){
    const Position pos = controllerGetGame()->position;
    int kingX = -1, kingY = -1;
    getKingPosition(&pos, pos.currentColor, &kingX, &kingY);
    if (isKingInCheck(&pos, pos.currentColor)){
        SDL_SetRenderDrawColor(renderer, 214, 54, 39, 255);
        SDL_Rect rect;
        rect.x = BOARD_ORIGIN_X + kingY*BOARD_CELL_SIZE;   
        rect.y = BOARD_ORIGIN_Y + kingX*BOARD_CELL_SIZE;  
        rect.w = BOARD_CELL_SIZE;   
        rect.h = BOARD_CELL_SIZE;   
        SDL_RenderFillRect(renderer, &rect);
    }
}

void draw_board_cells(SDL_Renderer *renderer){
    
    for(int i=0; i<8; i++){
        for(int j=0; j<8; j++){

            if((i%2) == (j%2)){
                SDL_SetRenderDrawColor(renderer, 239, 216, 180, 255);
            }
            else{
                SDL_SetRenderDrawColor(renderer, 180, 135, 98, 255);
            }

            SDL_Rect rect;
            rect.x = BOARD_ORIGIN_X + i*BOARD_CELL_SIZE;   
            rect.y = BOARD_ORIGIN_Y + j*BOARD_CELL_SIZE;  
            rect.w = BOARD_CELL_SIZE;   
            rect.h = BOARD_CELL_SIZE;   

            SDL_RenderFillRect(renderer, &rect);
        }
    }
}


void draw_board_filter(SDL_Renderer *renderer){
    SDL_Rect filter;
    filter.x = BOARD_ORIGIN_X;
    filter.y = BOARD_ORIGIN_Y;
    filter.w = BOARD_SIZE;   
    filter.h = BOARD_SIZE;   

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 100);
    SDL_RenderFillRect(renderer, &filter);
}


void draw_board_pieces_from_model(SDL_Renderer *renderer) {
    const Position pos = controllerGetGame()->position;

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {

            Cell cell = pos.board_model[i][j];

            if (cell.color == COLOR_WHITE) {
                if (cell.piece == PIECE_PAWN)
                    draw_a_piece_on_the_board(renderer, white_pawn_texture, i, j);
                else if (cell.piece == PIECE_ROOK)
                    draw_a_piece_on_the_board(renderer, white_rook_texture, i, j);
                else if (cell.piece == PIECE_KNIGHT)
                    draw_a_piece_on_the_board(renderer, white_knight_texture, i, j);
                else if (cell.piece == PIECE_BISHOP)
                    draw_a_piece_on_the_board(renderer, white_bishop_texture, i, j);
                else if (cell.piece == PIECE_QUEEN)
                    draw_a_piece_on_the_board(renderer, white_queen_texture, i, j);
                else if (cell.piece == PIECE_KING)
                    draw_a_piece_on_the_board(renderer, white_king_texture, i, j);
            }
            else if (cell.color == COLOR_BLACK) {
                if (cell.piece == PIECE_PAWN)
                    draw_a_piece_on_the_board(renderer, black_pawn_texture, i, j);
                else if (cell.piece == PIECE_ROOK)
                    draw_a_piece_on_the_board(renderer, black_rook_texture, i, j);
                else if (cell.piece == PIECE_KNIGHT)
                    draw_a_piece_on_the_board(renderer, black_knight_texture, i, j);
                else if (cell.piece == PIECE_BISHOP)
                    draw_a_piece_on_the_board(renderer, black_bishop_texture, i, j);
                else if (cell.piece == PIECE_QUEEN)
                    draw_a_piece_on_the_board(renderer, black_queen_texture, i, j);
                else if (cell.piece == PIECE_KING)
                    draw_a_piece_on_the_board(renderer, black_king_texture, i, j);
            }
        }
    }
}


void draw_board(SDL_Renderer *renderer) {
    if (!getAreTexturesLoaded()) {
        pieces_textures_init(renderer);
    }
    draw_board_cells(renderer);
    draw_king_check(renderer);
    draw_highlight_cell(renderer);
    draw_board_pieces_from_model(renderer);
    if(!boardClickEnabled){
        draw_board_filter(renderer);
    }
}

void setBoardClick(bool b){
    boardClickEnabled = b;
}

void handle_board_click(SDL_Event *event) {
    if (boardClickEnabled) { 
        if (event->type == SDL_MOUSEBUTTONDOWN) {
            int x = event->button.x;
            int y = event->button.y;

            int col = (x - BOARD_ORIGIN_X) / BOARD_CELL_SIZE;
            int row = (y - BOARD_ORIGIN_Y) / BOARD_CELL_SIZE;

            if (y >= BOARD_ORIGIN_Y && y < (BOARD_ORIGIN_Y+BOARD_SIZE) && x >= BOARD_ORIGIN_X && x < (BOARD_ORIGIN_X+BOARD_SIZE)) {
                selectCell(row, col);
            }
        }
    }
}