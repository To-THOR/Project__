#include "ch.h"
#include "hal.h"
#include <main.h>
#include <mode.h>
#include <motors.h>
#include <displacement.h>
#include <figure.h>


// --------------------------- CONSTANTS ---------------------------

#define RESET_LEFT_POS_MOTOR		0
#define RESET_RIGHT_POS_MOTOR		0
#define INITIAL_SPEED_LEFT			0
#define INITIAL_SPEED_RIGHT			0
#define NO_SPEED					0
#define NORMAL_SPEED				700 		//CAUTION: speed must have zero dozens and units (e.g. 100, 200, ...)
#define WHEEL_PERIMETER				1300 									//[100um]
#define	WHEEL_GAP					53										//[mm]
#define DISTANCE_FACTOR				(int)(WHEEL_PERIMETER/100)				//dist = (stepL+stepR)/(2*1000)   * perimeter
																			//dist = (stepL+stepR)/20 * perimeter/100
#define ALPHA_FACTOR_1				(float)(2*FIGURE_SIZE_1/WHEEL_GAP)
#define ALPHA_FACTOR_2				(float)(2*FIGURE_SIZE_2/WHEEL_GAP)
#define ALPHA_FACTOR_MAX			(float)(2*FIGURE_SIZE_MAX/WHEEL_GAP)
#define	CIRCLE_SPEED_FACTOR_1		(int)(100*(ALPHA_FACTOR_1+1)/(ALPHA_FACTOR_1-1))
#define	CIRCLE_SPEED_FACTOR_2		(int)(100*(ALPHA_FACTOR_2+1)/(ALPHA_FACTOR_2-1))
#define	CIRCLE_SPEED_FACTOR_MAX		(int)(100*(ALPHA_FACTOR_MAX+1)/(ALPHA_FACTOR_MAX-1))
#define	CIRCLE_SPEED_1				(int32_t)(NORMAL_SPEED*CIRCLE_SPEED_FACTOR_1)
#define	CIRCLE_SPEED_2				(int32_t)(NORMAL_SPEED*CIRCLE_SPEED_FACTOR_2)
#define	CIRCLE_SPEED_MAX			(int32_t)(NORMAL_SPEED*CIRCLE_SPEED_FACTOR_MAX)

// --------------------------- VARIABLES ---------------------------


/*
 * Name: 		-static_speed_left
 * Description:	-speed of the left wheel of the robot [step/sec]
 * 				-CAUTION: the maximum angle is 1100 step/sec
 * 				-CAUTION: the velocity must have zero dozens and units (only 100,200,300,... are acceptable)
 *
 * */
static	uint16_t static_speed_left 	= INITIAL_SPEED_LEFT;

/*
 * Name: 		-static_speed_right
 * Description:	-speed of the right wheel of the robot [step/sec]
 * 				-CAUTION: the maximum angle is 1100 step/sec
 * 				-CAUTION: the velocity must have zero dozens and units (only 100,200,300,... are acceptable)
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

void displacement_rotation(int32_t speed)
{
	right_motor_set_speed(speed);
	left_motor_set_speed(-speed);
}

void displacement_straight_speed_set(int32_t speed){
	if(speed == NORMAL_SPEED){
		left_motor_set_speed(NORMAL_SPEED);
		right_motor_set_speed(NORMAL_SPEED);
	}
	else{
		left_motor_set_speed(NO_SPEED_LEFT);
		right_motor_set_speed(NO_SPEED_RIGHT);
	}
}

void displacement_starting_point(void)
{
	displacement_straight_speed_set(NORMAL_SPEED);
	if (CONVERT*displacement_distance_get()==figure_size_get())
	{
		displacement_straight_speed_set(SPEED_NULL);
		// demi tour pour s'orienter sur la trajectoire
	}
}

void displacement_circle_speed(void){
	switch(figure_get_size()){
	case FIGURE_NO_SIZE:{
		right_motor_set_speed(NO_SPEED);
		left_motor_set_speed(NO_SPEED);
		break;
	}
	case FIGURE_SIZE_1:{
		left_motor_set_speed(NORMAL_SPEED);
		right_motor_set_speed(CIRCLE_SPEED_1);
		break;
	}
	case FIGURE_SIZE_2:{
		left_motor_set_speed(NORMAL_SPEED);
		right_motor_set_speed(CIRCLE_SPEED_2);
		break;
	}
	case FIGURE_SIZE_MAX:{
		left_motor_set_speed(NORMAL_SPEED);
		right_motor_set_speed(CIRCLE_SPEED_MAX);
		break;
	}
	default:{
		left_motor_set_speed(NO_SPEED);
		right_motor_set_speed(NO_SPEED);
	}
}
}
