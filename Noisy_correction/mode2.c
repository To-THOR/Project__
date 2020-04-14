 #include <p30F6014A.h>
 #include <e_motors.h>
 #include <motor_led/e_epuck_ports.h>
 #include <motor_led/e_init_port.h>
 #include <motor_led/advance_one_timer/e_motors.h>
 #include <motor_led/advance_one_timer/e_agenda.h>
/*An agenda is a structure made to work as chained list. It contains:
 * the function you want to launch, the time setup between two launching events,
 * a counter to measure the current time, a pointer to the next element of the list.
 * Each time the timer2 has an interrupt, all the agenda chained list is
 * scanned to look if an agenda has to be treated according to the cycle value
 * and current counter value.
 * If one (or more) agenda has to be treated, his callback function is launch.*/

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
 }
 while(1) {}
 }
