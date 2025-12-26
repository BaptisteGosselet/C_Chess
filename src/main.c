#include "gui/gui.h"
#include "model/position.h"
#include "controller/position_controller/position_controller.h"
#include "controller/main_controller.h"

int main(void) {
    init_game();         
    open_window();  
    destroy_game();          
    return 0;
}
