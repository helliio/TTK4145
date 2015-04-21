#include "elev.h"
#include <stdio.h>
#include <stdlib.h>

int io_pannels[3][N_FLOORS];
int move_dir = 0;
int location = -1;

void initialize_io_pannel(void){
    int i, j;
    for (i=0; i<3; i++){
        for (j=0; j<N_FLOORS; j++){
            io_pannels[i][j]=0;
        }
    }
}

void move_elevator(int dir){
    
}

void open_and_close_door(void){
    elev_set_door_open_lamp(1);
    delay(15000);
    elev_set_door_open_lamp(0);
    return;
}

void stop_elev_at_next_floor(void){
    int i = elev_get_floor_sensor_signal();
    if (i != -1) {
        elev_set_motor_direction(DIRN_STOP);
        open_and_close_door();
        turn_off_lights_at_floor(i);
    }
}

void turn_off_lights_at_floor(int i){
    if (i >= 0 && i <= N_FLOORS-1) {
        if(i < N_FLOORS-1){ 
            elev_set_button_lamp(BUTTON_CALL_UP, i, 0);
        }
        if(i > 0){
            elev_set_button_lamp(BUTTON_CALL_DOWN, i, 0);
        }
        elev_set_button_lamp(BUTTON_COMMAND, i, 0);
    }
}


void set_elev_floor_lamp(void){
    int i = elev_get_floor_sensor_signal();
    if (i != -1) {
        elev_set_floor_indicator(i);
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
        if(i < N_FLOORS-1){
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
