
#include <p30F6014A.h>
#include <motor_led/e_epuck_ports.h>
#include <motor_led/e_init_port.h>
#include <motor_led/advance_one_timer/e_motors.h>
#include <motor_led/advance_one_timer/e_agenda.h>

  int main(void)
  {
 	e_init_port();
 	e_init_motors();
    e_set_speed(-500, 500);
  	e_start_agendas_processing();
  	while(1) {}
  }


