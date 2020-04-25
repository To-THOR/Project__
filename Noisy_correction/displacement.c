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
#define STEPS_PER_REVOLUTION		1000
#define WHEEL_PERIMETER				130 									//[mm]
#define	WHEEL_GAP					53										//[mm] distance between wheels
#define DISTANCE_FACTOR				(int)(WHEEL_PERIMETER/100)				//dist = (stepL+stepR)/(2*1000)   * perimeter
																			//dist = (stepL+stepR)/20 * perimeter/100
#define	MM_TO_STEP_FACTOR			((float)STEPS_PER_REVOLUTION/(float)WHEEL_PERIMETER)
#define DISTANCE_1_STEP				(int)(FIGURE_SIZE_1*MM_TO_STEP_FACTOR)
#define DISTANCE_2_STEP				(int)(FIGURE_SIZE_2*MM_TO_STEP_FACTOR)
#define DISTANCE_MAX_STEP			(int)(FIGURE_SIZE_MAX*MM_TO_STEP_FACTOR)
#define ALPHA_FACTOR_1				(float)(2*FIGURE_SIZE_1/WHEEL_GAP)
#define ALPHA_FACTOR_2				(float)(2*FIGURE_SIZE_2/WHEEL_GAP)
#define ALPHA_FACTOR_MAX			(float)(2*FIGURE_SIZE_MAX/WHEEL_GAP)
#define	CIRCLE_SPEED_FACTOR_1		(int)(100*(ALPHA_FACTOR_1+1)/(ALPHA_FACTOR_1-1))
#define	CIRCLE_SPEED_FACTOR_2		(int)(100*(ALPHA_FACTOR_2+1)/(ALPHA_FACTOR_2-1))
#define	CIRCLE_SPEED_FACTOR_MAX		(int)(100*(ALPHA_FACTOR_MAX+1)/(ALPHA_FACTOR_MAX-1))
#define	CIRCLE_SPEED_1				(int32_t)(NORMAL_SPEED*CIRCLE_SPEED_FACTOR_1)
#define	CIRCLE_SPEED_2				(int32_t)(NORMAL_SPEED*CIRCLE_SPEED_FACTOR_2)
#define	CIRCLE_SPEED_MAX			(int32_t)(NORMAL_SPEED*CIRCLE_SPEED_FACTOR_MAX)
#define STEP_EQUALITY_ERROR			5

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
uint8_t displacement_straight_distance_check(int16_t distance_mm){
	if(right_motor_get_pos() >= left_motor_get_pos() - STEP_EQUALITY_ERROR &&
			right_motor_get_pos() <= left_motor_get_pos() + STEP_EQUALITY_ERROR){
		switch(distance_mm){
		case FIGURE_SIZE_1:
			if(right_motor_get_pos() >= DISTANCE_1_STEP) return DISTANCE_REACHED;
			else return DISTANCE_NOT_REACHED;
			break;
		case FIGURE_SIZE_2:
			if(right_motor_get_pos() >= DISTANCE_2_STEP) return DISTANCE_REACHED;
			else return DISTANCE_NOT_REACHED;
			break;
		case FIGURE_SIZE_MAX:
			if(right_motor_get_pos() >= DISTANCE_MAX_STEP) return DISTANCE_REACHED;
			else return DISTANCE_NOT_REACHED;
			break;
		default:
			return DISTANCE_NOT_REACHED;
		}
	}
	else return(DISTANCE_NOT_REACHED);
}


uint8_t displacement_rotation_angle_check(int16_t angle_degrees){
	switch(angle_degrees){
	case ANGLE_90_DEGREES :
		if (right_motor_get_pos()- left_motor_get_pos()>= ANGLE_90_STEP)
			return ANGLE_REACHED;
		else
			return ANGLE_NOT_REACHED;
		break;
	case ANGLE_360_DEGREES :
		if (right_motor_get_pos()- left_motor_get_pos() >= ANGLE_360_STEP)
			return ANGLE_REACHED;
		else
			return ANGLE_NOT_REACHED;
		break;
	case  ANGLE_SQUARE_DEGREES:
		if (right_motor_get_pos()- left_motor_get_pos() >= ANGLE_SQUARE_STEP)
			return ANGLE_REACHED;
		else
			return ANGLE_NOT_REACHED;
		break;
	case  ANGLE_TRIANGLE_DEGREES:
		if (right_motor_get_pos()- left_motor_get_pos() >= ANGLE_TRIANGLE_STEP)
			return ANGLE_REACHED;
		else
			return ANGLE_NOT_REACHED;
		break;
	default:
		return ANGLE_NOT_REACHED;
	}
}


void displacement_distance_reset(void){
	left_motor_set_pos(RESET_LEFT_POS_MOTOR);
	right_motor_set_pos(RESET_RIGHT_POS_MOTOR);
}


void displacement_angle_reset(void){
	left_motor_set_pos(RESET_LEFT_POS_MOTOR);
	right_motor_set_pos(RESET_RIGHT_POS_MOTOR);
}


void displacement_rotation(int32_t speed)
{
	if(speed == NORMAL_SPEED){
		right_motor_set_speed(NORMAL_SPEED);
		left_motor_set_speed(-NORMAL_SPEED);
	}
	else{
		left_motor_set_speed(NO_SPEED);
		right_motor_set_speed(NO_SPEED);
	}
}


void displacement_straight_speed_set(int32_t speed){
	if(speed == NORMAL_SPEED){
		left_motor_set_speed(NORMAL_SPEED);
		right_motor_set_speed(NORMAL_SPEED);
	}
	else{
		left_motor_set_speed(NO_SPEED);
		right_motor_set_speed(NO_SPEED);
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
