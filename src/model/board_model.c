#include "cell.h"

Cell board_model[8][8];

const Cell EMPTY_CELL = (Cell){-1,'\0'};

void init_board() {
    board_model[0][0] = (Cell){1,'r'};
    board_model[0][1] = (Cell){1,'n'};
    board_model[0][2] = (Cell){1,'b'};
    board_model[0][3] = (Cell){1,'q'};
    board_model[0][4] = (Cell){1,'k'};
    board_model[0][5] = (Cell){1,'b'};
    board_model[0][6] = (Cell){1,'n'};
    board_model[0][7] = (Cell){1,'r'};

    for(int i=0;i<8;i++)
        board_model[1][i] = (Cell){1,'p'};

    for(int i=2;i<6;i++)
        for(int j=0;j<8;j++)
            board_model[i][j] = (Cell){-1,'\0'};

    for(int i=0;i<8;i++)
        board_model[6][i] = (Cell){0,'p'};

    board_model[7][0] = (Cell){0,'r'};
    board_model[7][1] = (Cell){0,'n'};
    board_model[7][2] = (Cell){0,'b'};
    board_model[7][3] = (Cell){0,'q'};
    board_model[7][4] = (Cell){0,'k'};
    board_model[7][5] = (Cell){0,'b'};
    board_model[7][6] = (Cell){0,'n'};
    board_model[7][7] = (Cell){0,'r'};
}


void init_game(){
    init_board();
}

void moveTo(int oX, int oY, int dX, int dY){
    if (oX<0 || oX>7 || oY<0 || oY>7 || dX<0 || dX>7 || dY<0 || dY>7) return;
    board_model[dX][dY] = board_model[oX][oY];
    board_model[oX][oY] = EMPTY_CELL;
    
}