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

void stop_elev_at_next_floor(void){
    int i = elev_get_floor_sensor_signal();
    if (i != -1) {
        elev_set_motor_direction(DIRN_STOP);
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

void move_elev_once(int dir){
    int i = elev_get_floor_sensor_signal();
    if (dir > 0) {
        if (i == N_FLOORS-1) {
            printf("Elev cant go up")
            return;
        }
        go_to_floor(i+1);
        return;
    }
    if (dir < 0) {
        if (i == 0) {
            printf("Elev cant fo down")
            return;
        }
        go_to_floor(i-1);
        return;
    }
}

void go_to_floor(int target){
    int i = elev_get_floor_sensor_signal();
    if (i == -1) {
        printf("elev not at floor");
        return;
    }
    if (i == target) {
        return;
    }
    if (i > target) {
        while (1) {
            int i = elev_get_floor_sensor_signal();
            if (i == target) {
                elev_set_motor_direction(DIRN_STOP);
                break;
            }
            elev_set_motor_direction(DIRN_DOWN);
        }
        return;
    }
    if (i < target) {
        while (1) {
            int i = elev_get_floor_sensor_signal();
            if (i == target) {
                elev_set_motor_direction(DIRN_STOP);
                break;
            }
            elev_set_motor_direction(DIRN_UP);
        }
        return;
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
