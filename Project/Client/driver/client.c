#include "elev.h"
#include <stdio.h>

void set_elev_floor_lamp(void){
    int i = elev_get_floor_sensor_signal();
    if (i != -1) {
        elev_set_floor_indicator(i);
        if (i < 3) {
            elev_set_button_lamp(BUTTON_CALL_UP, i, 0);
        }
        if (i > 0) {
            elev_set_button_lamp(BUTTON_CALL_DOWN, i, 0);
        }
        elev_set_button_lamp(BUTTON_COMMAND, i, 0);
    }
}

void send_elevator_call_signal(void){
    int i;
    for (i = 0; i < N_FLOORS; i++){
        if(i < 3){
            if(elev_get_button_signal(BUTTON_CALL_UP, i) == 1){
                elev_set_button_lamp(BUTTON_CALL_UP, i, 1);
                printf("Button UP floor ",i+1," Pushed\n");
            }
        }
        if(i > 0){
            if(elev_get_button_signal(BUTTON_CALL_DOWN, i) == 1){
                elev_set_button_lamp(BUTTON_CALL_DOWN, i, 1);
                printf("Button Down floor ",i+1," Pushed\n");
            }
        }
        if(elev_get_button_signal(BUTTON_COMMAND, i) == 1){
            elev_set_button_lamp(BUTTON_COMMAND, i, 1);
            printf("Button COMMAND floor ",i+1," Pushed\n");
        }
    }
}
