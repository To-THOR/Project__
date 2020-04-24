#include "ch.h"
#include "hal.h"
#include <main.h>
#include <mode.h>
#include <motors.h>
#include <displacement.h>


// --------------------------- CONSTANTS ---------------------------

#define RESET_LEFT_POS_MOTOR		0
#define RESET_RIGHT_POS_MOTOR		0
#define INITIAL_SPEED_LEFT			0
#define INITIAL_SPEED_RIGHT			0
#define WHEEL_PERIMETER				1300 						//[100um]
#define DISTANCE_FACTOR				(int)(WHEEL_PERIMETER/100)	//dist = (stepL+stepR)/(2*1000)   * perimeter
																//dist = (stepL+stepR)/20 * perimeter/100

// --------------------------- VARIABLES ---------------------------


/*
 * Name: 		-static_speed_left
 * Description:	-speed of the left wheel of the robot [step/sec]
 * 				-CAUTION: the maximum angle is 1100 step/sec
 *
 * */
static	uint16_t static_speed_left 	= INITIAL_SPEED_LEFT;

/*
 * Name: 		-static_speed_right
 * Description:	-speed of the right wheel of the robot [step/sec]
 * 				-CAUTION: the maximum angle is 1100 step/sec
 *
 * */
static	uint16_t static_speed_right = INITIAL_SPEED_RIGHT;


// --------------------------- EXTERNAL FUNCTIONS ---------------------------

/*
 * Name: 		-displacement_start
 * Description:	-initializes the threads (motors and distance processing)
 * Arguments:	-void
 * Return:		-void
 *
 * */
void displacement_start(void){
	motors_init();
}


/*
 * Name: 		-distance_update
 * Description:	-updates the distance done by the robot according to the wheels velocities
 * 				-CAUTION: distance resolution: 19step/1000 * perimeter = 2.4 [mm]
 * 				-CAUTION: maximum distance is 2^16[100um] = 6.5 [m]
 * Arguments:	-void
 * Return:		-void
 *
 * */
uint16_t displacement_distance_get(void){
	return	DISTANCE_FACTOR * (int)((right_motor_get_pos() + left_motor_get_pos())/(int)20);
}

void displacement_distance_reset(void){
	left_motor_set_pos(RESET_LEFT_POS_MOTOR);
	right_motor_set_pos(RESET_RIGHT_POS_MOTOR);
}
