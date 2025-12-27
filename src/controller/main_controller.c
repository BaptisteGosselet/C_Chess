#include "../gui/gui.h"
#include "../model/position.h"
#include "position_controller/position_controller.h"
#include "../model/move.h"
#include "legal_move/legal_move.h"
#include <stdio.h>
#include <stdbool.h>
#include "../model/game.h"
#include <stdlib.h>
#include "../model/player_type.h"
#include "menu_params_controller/menu_params_controller.h"

static int selectedCell[2] = {-1, -1};

static Game *game = NULL;

void destroy_game(){
    free(game);
}

void init_game(PlayerType whiteType, PlayerType blackType) {
    if(game){
        destroy_game();
    }
    game = malloc(sizeof(Game));
    if (!game) return; 

    init_position(&game->position);
    game->whitePlayerType = whiteType;
    game->blackPlayerType = blackType;

    letComputerPlay(game);
}

void init_params_game(){
    init_game(paramWhitePlayerType, paramBlackPlayerType);
}

void init_default_game() {
    init_game(PLAYER_HUMAN, PLAYER_HUMAN);
}

static void resetSelectedCell() {
    selectedCell[0] = -1;
    selectedCell[1] = -1;
}

static void playSelectedCells(int oI, int oJ, int dI, int dJ) {
    Move wantedMove = (Move){
        .fromX = oI,
        .fromY = oJ,
        .toX   = dI,
        .toY   = dJ,
        .promotion = 0 // TODO: promotion
    };

    if (!isThisMoveLegal(wantedMove, &game->position.currentLegalMovesList)) {
        printf("Move illegal\n");
        resetSelectedCell();
        updateRender();
        return;
    }

    moveTo(game, oI, oJ, dI, dJ);

    printf("MOVE %d,%d -> %d,%d\n", oI, oJ, dI, dJ);

    resetSelectedCell();
    updateRender();
}

void selectCell(int i, int j) {
    if(game->position.isFinal){
        return;
    }
    
    if (selectedCell[0] == -1) {
        selectedCell[0] = i;
        selectedCell[1] = j;
        updateRender();
        printf("Case sélectionnée %d,%d\n", i, j);
        return;
    }

    if (selectedCell[0] == i && selectedCell[1] == j) {
        resetSelectedCell();
        updateRender();
        printf("Case annulée %d,%d\n", i, j);
        return;
    }

    playSelectedCells(selectedCell[0], selectedCell[1], i, j);
}

const Game* controllerGetGame(void) {
    return game;
}

const int* controllerGetSelectedCell(void) {
    return selectedCell;
}