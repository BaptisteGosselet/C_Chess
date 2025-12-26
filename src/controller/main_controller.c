#include "../gui/gui.h"
#include "../model/position.h"
#include "../model/move.h"
#include "legal_move/legal_move.h"
#include <stdio.h>
#include <stdbool.h>

static int selectedCell[2] = {-1, -1};
static Position *game = NULL;

void controllerInit(Position *pos) {
    game = pos;
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
        .promotion = 0 // TODO promotion
    };

    if (!isThisMoveLegal(wantedMove, &game->currentLegalMovesList)) {
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
    if(game->isFinal){
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

const Position* controllerGetPosition(void) {
    return game;
}

const int* controllerGetSelectedCell(void) {
    return selectedCell;
}