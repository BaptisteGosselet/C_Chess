#include "cell.h"

Cell board_model[8][8];

const Cell (*getBoard(void))[8] {
    return board_model; // retourne un pointeur vers le tableau original en lecture seule (à vérifier)
}


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
