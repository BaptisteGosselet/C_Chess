#include "../gui/gui.h"
#include "../model/board_model.h"
#include <stdio.h>

int selectedCell[2] = {-1, -1};

void resetSelectedCell(){
    selectedCell[0] = -1;
    selectedCell[1] = -1;
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

    //tmp
    moveTo(selectedCell[0], selectedCell[1], i, j);
    printf("MOVE  %d, %d to %d, %d \n", selectedCell[0], selectedCell[1], i, j);
    resetSelectedCell();
    updateRender();
    return;

}