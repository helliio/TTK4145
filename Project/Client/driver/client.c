#include "elev.h"
#include <stdio.h>
#include <stdlib.h>

int commands[3][N_FLOORS];
int move_dir = 0;

void initialize_io_pannel(void){
    int i, j;
    for (i=0; i<3; i++){
        for (j=0; j<N_FLOORS; j++){
            commands[i][j]=0;
        }
    }
}

void move_elevator(int dir){
    int i = elev_get_floor_sensor_signal();
    if (i != -1) {
        if (dir > 0 && i < N_FLOORS-1) {
            elev_set_motor_direction(DIRN_UP);
            return;
        }
        if (dir == 0) {
            elev_set_motor_direction(DIRN_STOP);
            return;
        }
        if (dir < 0 && i > 0) {
            elev_set_motor_direction(DIRN_DOWN);
            return;
        }
    }
    if (i == -1) {
        if (dir > 0) {
            elev_set_motor_direction(DIRN_UP);
            return;
        }
        if (dir == 0) {
            elev_set_motor_direction(DIRN_STOP);
            return;
        }
        if (dir < 0) {
            elev_set_motor_direction(DIRN_DOWN);
            return;
        }
    }
}

void execute_orders(void){
    int i;
    int commands = 0;
    int loc = elev_get_floor_sensor_signal();
    if (loc == -1) {
        return;
    }
    for (i=0;i<N_FLOORS-1;i++) {
        if (commands[0][i]==1 || commands[1][i]==1 || commands[2][i]==1) {
            commands = 1;
        }
        if (move_dir == 0) {
            if (commands[0][i]==1 || commands[1][i]==1 || commands[2][i]==1) {
                if (loc > i) {
                    move_dir = -1;
                    move_elevator(-1);
                    return;
                }
                if (loc < i) {
                    move_dir = 1;
                    move_elevator(1);
                    return;
                }
                if (loc == i) {
                    stop_and_open();
                    return;
                }
            }
        }
        if (move_dir > 0) {
            if (loc == N_FLOORS) {
                stop_and_open();
                move_dir = 0;
                return;
            }
            if (commands[0][i]==1 && loc == i) {
                stop_and_open();
                move_elevator(1);
                return;
            }
            if (commands[1][i]==1 && loc == i) {
                stop_and_open();
                move_elevator(1);
                return;
            }
            
        }
        if (move_dir < 0) {
            if (loc == 0) {
                stop_and_open();
                move_dir = 0; 
            }
            if (commands[0][i]==1 && loc == i) {
                stop_and_open();
                move_elevator(-1);
                return;
            }
            if (commands[2][i]==1 && loc == i) {
                stop_and_open();
                move_elevator(-1);
                return;
            }
        }
    }
    if (commands == 0) {
        elev_set_motor_direction(DIRN_STOP);
        move_dir = 0; 
    }   
}

void stop_and_open(void){
    int i = elev_get_floor_sensor_signal();
    elev_set_motor_direction(DIRN_STOP);
    turn_off_lights_at_floor(i);
    elev_set_door_open_lamp(1);
    delay(10000);
    elev_set_door_open_lamp(0);
    if (move_dir == 0) {
        commands[0][i] = 0;
        commands[1][i] = 0;
        commands[2][i] = 0;
    }
    if (move_dir > 0) {
        commands[0][i] = 0;
        commands[1][i] = 0;
    }
    if (move_dir < 0) {
        commands[0][i] = 0;
        commands[2][i] = 0;
    }

    return;
}

void turn_off_lights_at_floor(int i){
    if (i >= 0 && i <= N_FLOORS-1) {
        if(i < N_FLOORS-1 && move_dir > 0){ 
            elev_set_button_lamp(BUTTON_CALL_UP, i, 0);
        }
        if(i > 0 && move_dir < 0){
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
            commands[0][i]=1;
            printf("Button COMMAND floor ",i+1," Pushed\n");
        }
        if(i < N_FLOORS-1){
            if(elev_get_button_signal(BUTTON_CALL_UP, i) == 1){
                elev_set_button_lamp(BUTTON_CALL_UP, i, 1);
                commands[1][i]=1;
                printf("Button UP floor ",i+1," Pushed\n");
            }
        }
        if(i > 0){
            if(elev_get_button_signal(BUTTON_CALL_DOWN, i) == 1){
                elev_set_button_lamp(BUTTON_CALL_DOWN, i, 1);
                commands[2][i]=1;
                printf("Button Down floor ",i+1," Pushed\n");
            }
        }
    }
}

void send_elevator_call_signal(void){
    return;
}
