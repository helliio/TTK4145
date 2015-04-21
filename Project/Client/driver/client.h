/**
Set floor lamps
*/
void set_elev_floor_lamp(void); 

/**
Send button signal to server
*/
void send_elevator_call_signal(void);

/**
Run once!
*/
void initialize_io_pannel(void);

/**
Listen to the IO panel
*/
void listen_to_io_panels(void);

/**
Stop the elevator at the next floor it passes
*/
void stop_elev_at_next_floor(void);

/**
Open and close door
*/

void open_and_close_door(void);

/**
Turn off light at designated floor
*/
void turn_off_lights_at_floor (int i);

/**
Move the elevator and it will stop at any floor that has a stop flag
*/
void move_elevator(int dir);