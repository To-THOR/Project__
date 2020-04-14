#include <stdio.h>
#include <stdlib.h>
#include <string.h>



// moteur 360°

#include <motor_led/e_epuck_ports.h>
#include <motor_led/e_init_port.h>
#include <motor_led/advance_one_timer/e_motors.h>
#include <motor_led/advance_one_timer/e_agenda.h>



int main(void)
{
	e_init_port();
	e_init_motors();
	//{This function manage the left motor speed by changing the MOTOR1 phases.
	//The changing phases frequency (=> speed) is controled by the agenda
	//param motor_speed from -1000 to 1000 give the motor speed in steps/s,
	//positive value to go forward and negative to go backward.}
	e_set_speed(-500, 500);
	//e_set_speed(int linear_speed, int angular_speed)
	e_start_agendas_processing();
	// { reset Timer2 CONtrol register
	// precsaler = 1
	// clear timer 2
	// interrupt every 0.1 ms with 64 prescaler
	// clear interrupt flag
	// set interrupt enable bit
	// start Timer2 }
while(1) {}
}

