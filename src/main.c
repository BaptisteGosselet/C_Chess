#include "gui/gui.h"
#include "model/position.h"
#include "controller/position_controller/position_controller.h"
#include "controller/main_controller.h"

int main(void) {
    Position game;
    init_game(&game);         
    controllerInit(&game);     
    open_window();            
    return 0;
}
