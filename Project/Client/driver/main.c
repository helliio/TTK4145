#include "elev.h"
#include "client.h"
#include <stdio.h>


int main() {
    // Initialize hardware
    if (!elev_init()) {
        printf("Unable to initialize elevator hardware!\n");
        return 1;
    }
    initialize_io_pannel();

    printf("Press STOP button to stop elevator and exit program.\n");

    while (1) {
        
        set_elev_floor_lamp();
        listen_to_io_panels();
        if(is_door_open(3) == 0){
        	execute_orders();
        }

        

        // Stop elevator and exit program if the stop button is pressed
        if (elev_get_stop_signal()) {
            elev_set_motor_direction(DIRN_STOP);
            break;
        }
    }

    return 0;
}
