#include "../gui/gui.h"
#include "../model/board_model.h"
#include "../model/move.h"
#include "../model/legal_move.h"
#include <stdio.h>
#include <stdbool.h>


int selectedCell[2] = {-1, -1};

void resetSelectedCell(){
    selectedCell[0] = -1;
    selectedCell[1] = -1;
}

void playSelectedCells(int oI, int oJ, int dI, int dJ){
    Move wantedMove = (Move){
            .fromX = oI,
            .fromY = oJ,
            .toX = dI,
            .toY = dJ,
            .promotion = 0 //todo (if pawn && i/j = 0/8) : ask prom
        };

    if (!isThisMoveLegal(wantedMove)) {
        printf("Move illegal\n");
        resetSelectedCell();
        return;
    }

    moveTo(oI, oJ, dI, dJ);
    printf("MOVE  %d, %d to %d, %d \n", oI, oJ, dI, dJ);
    resetSelectedCell();
    updateRender();
}

void selectCell(int i, int j){
    
    if(selectedCell[0] == -1 || selectedCell[1] == -1){
        selectedCell[0] = i;
        selectedCell[1] = j;
        updateRender();
        printf("Case selectionnée %d, %d\n", i, j);
        return;
    }

    if(selectedCell[0] == i && selectedCell[1] == j){
        resetSelectedCell();
        updateRender();
        printf("Case annulée %d, %d\n", i, j);
        return;
    }

    playSelectedCells(selectedCell[0], selectedCell[1], i, j);
}