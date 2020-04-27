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
#define PI							(float)(3.14159)
#define STEPS_PER_REVOLUTION		1000
#define WHEEL_PERIMETER				130 									//[mm]
#define	WHEEL_GAP					53										//[mm] distance between wheels
#define	MM_TO_STEP_FACTOR				((float)STEPS_PER_REVOLUTION/(float)WHEEL_PERIMETER)
#define DEGREES_TO_STEP_FACTOR			(float)((STEPS_PER_REVOLUTION*WHEEL_GAP*2*PI)/(360*WHEEL_PERIMETER))
#define DISTANCE_1_STEP					(uint16_t)(FIGURE_SIZE_1*MM_TO_STEP_FACTOR)
#define DISTANCE_2_STEP					(uint16_t)(FIGURE_SIZE_2*MM_TO_STEP_FACTOR)
#define DISTANCE_MAX_STEP				(uint16_t)(FIGURE_SIZE_MAX*MM_TO_STEP_FACTOR)
#define DISTANCE_SQUARE_SIDE_1_STEP		(uint16_t)(SQUARE_SIDE_1*MM_TO_STEP_FACTOR)
#define DISTANCE_SQUARE_SIDE_2_STEP		(uint16_t)(SQUARE_SIDE_2*MM_TO_STEP_FACTOR)
#define DISTANCE_SQUARE_SIDE_MAX_STEP	(uint16_t)(SQUARE_SIDE_MAX*MM_TO_STEP_FACTOR)
#define DISTANCE_TRIANGLE_SIDE_1_STEP	(uint16_t)(TRIANGLE_SIDE_1*MM_TO_STEP_FACTOR)
#define DISTANCE_TRIANGLE_SIDE_2_STEP	(uint16_t)(TRIANGLE_SIDE_2*MM_TO_STEP_FACTOR)
#define DISTANCE_TRIANGLE_SIDE_MAX_STEP	(uint16_t)(TRIANGLE_SIDE_MAX*MM_TO_STEP_FACTOR)
#define ALPHA_FACTOR_1					(float)(2*FIGURE_SIZE_1/WHEEL_GAP)
#define ALPHA_FACTOR_2					(float)(2*FIGURE_SIZE_2/WHEEL_GAP)
#define ALPHA_FACTOR_MAX				(float)(2*FIGURE_SIZE_MAX/WHEEL_GAP)
#define	CIRCLE_SPEED_FACTOR_1			(int)(100*(ALPHA_FACTOR_1+1)/(ALPHA_FACTOR_1-1))
#define	CIRCLE_SPEED_FACTOR_2			(int)(100*(ALPHA_FACTOR_2+1)/(ALPHA_FACTOR_2-1))
#define	CIRCLE_SPEED_FACTOR_MAX			(int)(100*(ALPHA_FACTOR_MAX+1)/(ALPHA_FACTOR_MAX-1))
#define	CIRCLE_SPEED_1					(uint32_t)(NORMAL_SPEED*CIRCLE_SPEED_FACTOR_1)
#define	CIRCLE_SPEED_2					(uint32_t)(NORMAL_SPEED*CIRCLE_SPEED_FACTOR_2)
#define	CIRCLE_SPEED_MAX				(uint32_t)(NORMAL_SPEED*CIRCLE_SPEED_FACTOR_MAX)
#define STEP_EQUALITY_ERROR				(uint16_t)(5)
#define	ANGLE_90_STEP					(uint16_t)(DEGREES_TO_STEP_FACTOR*ANGLE_90_DEGREES)
#define	ANGLE_60_STEP					(uint16_t)(DEGREES_TO_STEP_FACTOR*ANGLE_60_DEGREES)
#define	ANGLE_360_STEP					(uint16_t)(DEGREES_TO_STEP_FACTOR*ANGLE_360_DEGREES)
#define	ANGLE_SQUARE_STEP				(uint16_t)(DEGREES_TO_STEP_FACTOR*ANGLE_SQUARE_DEGREES)
#define	ANGLE_TRIANGLE_STEP				(uint16_t)(DEGREES_TO_STEP_FACTOR*ANGLE_TRIANGLE_DEGREES)


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
 * 				-CAUTION: maximum distance is 2^16[step] = 8.5 [m]
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
		case SQUARE_SIDE_1:
			if(right_motor_get_pos() >= DISTANCE_SQUARE_SIDE_1_STEP) return DISTANCE_REACHED;
			else return DISTANCE_NOT_REACHED;
			break;
		case SQUARE_SIDE_2:
			if(right_motor_get_pos() >= DISTANCE_SQUARE_SIDE_2_STEP) return DISTANCE_REACHED;
			else return DISTANCE_NOT_REACHED;
			break;
		case SQUARE_SIDE_MAX:
			if(right_motor_get_pos() >= DISTANCE_SQUARE_SIDE_MAX_STEP) return DISTANCE_REACHED;
			else return DISTANCE_NOT_REACHED;
			break;
		case TRIANGLE_SIDE_1:
			if(right_motor_get_pos() >= DISTANCE_TRIANGLE_SIDE_1_STEP) return DISTANCE_REACHED;
			else return DISTANCE_NOT_REACHED;
			break;
		case TRIANGLE_SIDE_2:
			if(right_motor_get_pos() >= DISTANCE_TRIANGLE_SIDE_2_STEP) return DISTANCE_REACHED;
			else return DISTANCE_NOT_REACHED;
			break;
		case TRIANGLE_SIDE_MAX:
			if(right_motor_get_pos() >= DISTANCE_TRIANGLE_SIDE_MAX_STEP) return DISTANCE_REACHED;
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
	case ANGLE_60_DEGREES:
		if((right_motor_get_pos()-left_motor_get_pos()) >= ANGLE_60_STEP) return ANGLE_REACHED;
		else return ANGLE_NOT_REACHED;
		break;
	case ANGLE_90_DEGREES:
		if (right_motor_get_pos()- left_motor_get_pos()>= ANGLE_90_STEP) return ANGLE_REACHED;
		else return ANGLE_NOT_REACHED;
		break;
	case ANGLE_360_DEGREES:
		if (right_motor_get_pos()- left_motor_get_pos() >= ANGLE_360_STEP)
			return ANGLE_REACHED;
		else return ANGLE_NOT_REACHED;
		break;
	case  ANGLE_SQUARE_DEGREES:
		if (right_motor_get_pos()- left_motor_get_pos() >= ANGLE_SQUARE_STEP) return ANGLE_REACHED;
		else return ANGLE_NOT_REACHED;
		break;
	case  ANGLE_TRIANGLE_DEGREES:
		if (right_motor_get_pos()- left_motor_get_pos() >= ANGLE_TRIANGLE_STEP) return ANGLE_REACHED;
		else return ANGLE_NOT_REACHED;
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
		right_motor_set_speed(NORMAL_ROT_SPEED);
		left_motor_set_speed(-NORMAL_ROT_SPEED);
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
	switch(figure_size_get()){
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
