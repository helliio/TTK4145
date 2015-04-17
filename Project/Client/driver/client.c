#include "elev.h"
#include <stdio.h>
#include <stdlib.h>

int io_pannels[3][N_FLOORS];

void initialize_io_pannel(void){
    int i, j;
    for (i=0; i<3; i++){
        for (j=0; j<N_FLOORS; j++){
            io_pannels[i][j]=0;
        }
    }
}

void set_elev_floor_lamp(void){
    int i = elev_get_floor_sensor_signal();
    if (i != -1) {
        elev_set_floor_indicator(i);
        elev_set_button_lamp(BUTTON_COMMAND, i, 0);
        io_pannels[0][i]=0;
    }
}

void listen_to_io_panels(void){
    int i;
    for (i = 0; i < N_FLOORS; i++){
        if(elev_get_button_signal(BUTTON_COMMAND, i) == 1){
            elev_set_button_lamp(BUTTON_COMMAND, i, 1);
            io_pannels[0][i]=1;
            printf("Button COMMAND floor ",i+1," Pushed\n");
        }
        if(i < 3){
            if(elev_get_button_signal(BUTTON_CALL_UP, i) == 1){
                elev_set_button_lamp(BUTTON_CALL_UP, i, 1);
                io_pannels[1][i]=1;
                printf("Button UP floor ",i+1," Pushed\n");
            }
        }
        if(i > 0){
            if(elev_get_button_signal(BUTTON_CALL_DOWN, i) == 1){
                elev_set_button_lamp(BUTTON_CALL_DOWN, i, 1);
                io_pannels[2][i]=1;
                printf("Button Down floor ",i+1," Pushed\n");
            }
        }
    }
}

void send_elevator_call_signal(void){
    return;
}
