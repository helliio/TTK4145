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
Tell the elevator to go to floor
*/
void go_to_floor(int target);