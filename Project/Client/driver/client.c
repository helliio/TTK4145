#include "elev.h"
#include "client.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <arpa/inet.h>
#include <signal.h>

int commands[3][N_FLOORS];
int move_dir = 0;
int var = 0;
time_t door_timer;
int sock;
char last_sent[20];
char last_button[20];

void initialize(void){
    int i, j;
    for (i=0; i<3; i++){
        for (j=0; j<N_FLOORS; j++){
            commands[i][j]=0;
        }
    }
    while (1){
        int floor = elev_get_floor_sensor_signal();
        if (floor == 0){
            elev_set_motor_direction(DIRN_STOP);
            break;
        }
        elev_set_motor_direction(DIRN_DOWN);
    }
    
    
    struct sockaddr_in server;

    //Create socket
    sock = socket(AF_INET , SOCK_STREAM , 0);
    if (sock == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");

    //server.sin_addr.s_addr = inet_addr("129.241.187.140");
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons(8080);

    //Connect to remote server
    if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        perror("connect failed. Error");
        return;
    }

    puts("Connected\n");
    
    pthread_t thread;
    pthread_create(&thread, NULL, receve, &var);
    

    send_location(1);
}

void act_message(char * message)
{
}

void *receve(void *arg)
{
	#define buf_len (256)
	#define buf_space (buf_len - buf_fill)
	#define buf_end (&buf[buf_fill])

	char buf[buf_len];
	int buf_fill = 0;

	// ensure the buffer can be treated as a string
	buf[0] = '\0';

	for(;;)
	{
		buf_fill += recv(sock, buf_end, buf_space, 0);

		char * next_line = strchr(buf, '\n') + 1;
		int line_len = next_line - buf;

		if(next_line)
		{
			act_message(buf);

			buf_fill -= line_len;
			memmove(&buf, next_line, buf_fill);
			buf[buf_fill] = 0;
		}
	}
}

void send_location(int stop){
    char floor[20];
    char direction[20];
    char output[20] = "loc ";
    char output_s[20] = "stop ";
    int i = elev_get_floor_sensor_signal();
    if (i != -1){
        if (stop == 1){
            snprintf(floor,20,"%d",i);
            snprintf(direction,20,"%d",move_dir);
            strcat(strcat(strcat(strcat(output_s, floor)," "),direction), "\n");
            if (strcmp(output_s, last_sent) != 0){
                strcpy(last_sent, output_s);
                printf(output_s);
                send(sock,output_s,strlen(output_s), 0);
            }
        }
        if (stop == 0){
            snprintf(floor,20,"%d",i);
            snprintf(direction,20,"%d",move_dir);
            strcat(strcat(strcat(strcat(output, floor)," "),direction), "\n");
            if (strcmp(output, last_sent) != 0){
                strcpy(last_sent, output);
                printf(output);
                send(sock,output,strlen(output), 0);
            }
        }
    }
    
}

void send_button(int floor, int dir){
    char loc[20];
    char direction[20];
    char output[20] = "btn ";
    snprintf(loc,20,"%d",floor);
    snprintf(direction,20,"%d",dir);
    strcat(strcat(strcat(strcat(output, loc)," "),direction), "\n");
    if (strcmp(output, last_button) != 0){
        strcpy(last_button, output);
        printf(output);
        send(sock,output,strlen(output), 0);
    }
}

int is_door_open(double open_time){
    time_t now;
    time(&now);
    double diff = difftime(now,door_timer);
    if (diff < open_time){
        elev_set_door_open_lamp(1);
        return 1;
    }
    else{
        if(elev_get_obstruction_signal()==1){
            return 1;
        }
        elev_set_door_open_lamp(0);
        return 0;
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

int should_stop(int floor){
    if (commands[0][floor] == 1 || (commands[1][floor] == 1 && move_dir >= 0) || (commands[2][floor] == 1 && move_dir <= 0)){
        //printf("stop %d\n", floor+1);
        return 1;
    }else{
        //printf("no stop\n");
        return 0;
    }
}

int could_stop(int floor){
    if (commands[0][floor] == 1 || commands[1][floor] == 1 || commands[2][floor] == 1){
        return 1;
    }else{
        return 0;
    }
}

void stop_if_done(int loc){
    int do_cont = 0;
    if (move_dir > 0){
        int i;
        for(i=loc+1;i<N_FLOORS;i++){
            if (could_stop(i)==1){
                do_cont = 1;
            }
        }
    }
    if (move_dir < 0){
        int i;
        for(i=0;i<loc;i++){
            if (could_stop(i)==1){
                //printf("please continue down\n");
                do_cont = 1;
            }
        }
    }
    if (do_cont == 0){
        if (move_dir != 0)
            //printf("nothing in this direction\n");
        move_dir = 0;
    }
}

void execute_orders(void){
    int loc = elev_get_floor_sensor_signal();
    if (loc != -1){
        turn_off_lights_at_floor(loc, move_dir);
        if(should_stop(loc) == 1){
            stop_and_open();
            stop_if_done(loc);
        }else if(move_dir==0){
            int i,j;
            for (i=0; i<3;i++){
                for(j=0;j<N_FLOORS;j++){
                    if (commands[i][j] == 1){
                        if (loc<j){
                            //printf("going up\n");
                            move_dir = 1;
                            move_elevator(1);
                            return;
                        }else{
                            //printf("going down\n");
                            move_dir = -1;
                            move_elevator(-1);
                            return;
                        }
                    }
                }
            }

        }else{
            stop_if_done(loc);
            //printf("direction %d\n", move_dir);
            if (loc == 0 && move_dir == -1){
                //printf("at the bottom\n");
                move_dir = 0;
            }
            if (loc == N_FLOORS-1 && move_dir == 1){
                //printf("at the top\n");
                move_dir = 0;
            }

            move_elevator(move_dir);
        }
    }
}

void stop_and_open(void){
    strcpy(last_button, "");
    send_location(1);
    int i = elev_get_floor_sensor_signal();
    elev_set_motor_direction(DIRN_STOP);
    set_elev_floor_lamp();
    time(&door_timer);
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

void turn_off_lights_at_floor(int i, int dir){
    if (i >= 0 && i <= N_FLOORS-1) {
        send_location(0);
        if (i == 0){
            elev_set_button_lamp(BUTTON_CALL_UP, i, 0);
        }
        if (i == N_FLOORS-1){
            elev_set_button_lamp(BUTTON_CALL_DOWN, i, 0);
        }
        if(i < N_FLOORS-1 && dir >= 0){ 
            elev_set_button_lamp(BUTTON_CALL_UP, i, 0);
        }
        if(i > 0 && dir <= 0){
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
        }
        if(i < N_FLOORS-1){
            if(elev_get_button_signal(BUTTON_CALL_UP, i) == 1){
                elev_set_button_lamp(BUTTON_CALL_UP, i, 1);
                send_button(i, 1);
                commands[1][i]=1;
            }
        }
        if(i > 0){
            if(elev_get_button_signal(BUTTON_CALL_DOWN, i) == 1){
                elev_set_button_lamp(BUTTON_CALL_DOWN, i, 1);
                send_button(i, -1);
                commands[2][i]=1;
            }
        }
    }
}
