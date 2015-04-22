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
Stop the motor and open-close the doors this also resets the command array
*/

void stop_and_open(void);

/**
Turn off light at designated floor
*/
void turn_off_lights_at_floor (int i);

/**
Move the elevator and it will stop at any floor that has a stop flag -1 down 1 up 0 stop.
*/
void move_elevator(int dir);

/**
Execute orders from the array
*/
void execute_orders(void);