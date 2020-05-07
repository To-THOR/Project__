// --------------------------- DESCRIPTION ---------------------------
//displacement.c
//file responsible for the distance and speed processing

// --------------------------- INCLUDES ---------------------------

#include "ch.h"
#include "hal.h"
#include <main.h>
#include <displacement.h>


// --------------------------- CONSTANTS ---------------------------

#define RESET_LEFT_POS_MOTOR			((int32_t)(0))
#define RESET_RIGHT_POS_MOTOR			((int32_t)(0))
#define PI								((float)(3.14159))

//Parameters of the robot
#define STEPS_PER_REVOLUTION			1000					//[step]
#define WHEEL_PERIMETER					130 					//[mm]
#define	WHEEL_GAP						53						//distance between wheels [mm]

//Factors
#define	MM_TO_STEP_FACTOR				((float)STEPS_PER_REVOLUTION/(float)WHEEL_PERIMETER)
#define DEGREES_TO_STEP_FACTOR			((float)((STEPS_PER_REVOLUTION*WHEEL_GAP*2*PI)/(360*WHEEL_PERIMETER)))
#define	CIRCLE_SPEED_FACTOR_1			((float)(1+((float)(2*WHEEL_GAP))/((float)(2*FIGURE_SIZE_1-WHEEL_GAP))))
#define	CIRCLE_SPEED_FACTOR_2			((float)(1+((float)(2*WHEEL_GAP))/((float)(2*FIGURE_SIZE_2-WHEEL_GAP))))
#define	CIRCLE_SPEED_FACTOR_MAX			((float)(1+((float)(2*WHEEL_GAP))/((float)(2*FIGURE_SIZE_MAX-WHEEL_GAP))))

//Distance and speed constant table
#define DISTANCE_1_STEP					((uint16_t)(((float)FIGURE_SIZE_1)*MM_TO_STEP_FACTOR))
#define DISTANCE_2_STEP					((uint16_t)(((float)FIGURE_SIZE_2)*MM_TO_STEP_FACTOR))
#define DISTANCE_MAX_STEP				((uint16_t)(((float)FIGURE_SIZE_MAX)*MM_TO_STEP_FACTOR))
#define DISTANCE_SQUARE_SIDE_1_STEP		((uint16_t)(((float)SQUARE_SIDE_1)*MM_TO_STEP_FACTOR))
#define DISTANCE_SQUARE_SIDE_2_STEP		((uint16_t)(((float)SQUARE_SIDE_2)*MM_TO_STEP_FACTOR))
#define DISTANCE_SQUARE_SIDE_MAX_STEP	((uint16_t)(((float)SQUARE_SIDE_MAX)*MM_TO_STEP_FACTOR))
#define DISTANCE_TRIANGLE_SIDE_1_STEP	((uint16_t)(((float)TRIANGLE_SIDE_1)*MM_TO_STEP_FACTOR))
#define DISTANCE_TRIANGLE_SIDE_2_STEP	((uint16_t)(((float)TRIANGLE_SIDE_2)*MM_TO_STEP_FACTOR))
#define DISTANCE_TRIANGLE_SIDE_MAX_STEP	((uint16_t)(((float)TRIANGLE_SIDE_MAX)*MM_TO_STEP_FACTOR))
#define CIRCLE_SPEED_LEFT_1				((float)((float)(MOTOR_SPEED_LIMIT)*((float)(2*FIGURE_SIZE_1-WHEEL_GAP)) \
		/((float)(2*FIGURE_SIZE_1+WHEEL_GAP))))
#define CIRCLE_SPEED_LEFT_2				((float)((float)(MOTOR_SPEED_LIMIT)*((float)(2*FIGURE_SIZE_2-WHEEL_GAP)) \
		/((float)(2*FIGURE_SIZE_2+WHEEL_GAP))))
#define CIRCLE_SPEED_LEFT_MAX				((float)((float)(MOTOR_SPEED_LIMIT)*((float)(2*FIGURE_SIZE_MAX-WHEEL_GAP)) \
		/((float)(2*FIGURE_SIZE_MAX+WHEEL_GAP))))
#define	CIRCLE_SPEED_RIGHT_1			((uint32_t)(((float)(CIRCLE_SPEED_LEFT_1))*CIRCLE_SPEED_FACTOR_1))
#define	CIRCLE_SPEED_RIGHT_2			((uint32_t)(((float)(CIRCLE_SPEED_LEFT_2))*CIRCLE_SPEED_FACTOR_2))
#define	CIRCLE_SPEED_RIGHT_MAX			((uint32_t)(((float)(CIRCLE_SPEED_LEFT_MAX))*CIRCLE_SPEED_FACTOR_MAX))
#define	ANGLE_90_STEP					((uint16_t)(DEGREES_TO_STEP_FACTOR*((float)(ANGLE_90_DEGREES))))
#define	ANGLE_120_STEP					((uint16_t)(DEGREES_TO_STEP_FACTOR*((float)(ANGLE_120_DEGREES))))
#define	ANGLE_360_STEP					((uint16_t)(DEGREES_TO_STEP_FACTOR*((float)(ANGLE_360_DEGREES))))
#define	ANGLE_SQUARE_STEP				((uint16_t)(DEGREES_TO_STEP_FACTOR*((float)(ANGLE_SQUARE_DEGREES))))
#define	ANGLE_TRIANGLE_STEP				((uint16_t)(DEGREES_TO_STEP_FACTOR*((float)(ANGLE_TRIANGLE_DEGREES))))

//Resolution for the angle check
#define STEP_EQUALITY_ERROR				((uint16_t)(35))


// --------------------------- EXTERNAL FUNCTIONS ---------------------------

/*
 * Name: 		-displacement_start
 * Description:	-initializes the threads related to displacement
 * Arguments:	-void
 * Return:		-void
 *
 * */
void displacement_start(void){
	motors_init();
}


/*
 * Name: 		-displacement_straight_distance_check
 * Description:	-checks if the distance given in parameters has been reached or excelled
 * 				-CAUTION: maximum distance is 2^16[step] = 8.5 [m] due to the motor structure implementation in motors.h
 * Arguments:	-int16_t distance_mm: distance that has to be checked [mm]
 * Return:		-boolean (CAUTION: 1 if not reached, 0 if reached)
 *
 * */
uint8_t displacement_straight_distance_check(int16_t distance_mm){
	if(right_motor_get_pos() >= left_motor_get_pos() - STEP_EQUALITY_ERROR &&		//initial check that the trajectory is
			right_motor_get_pos() <= left_motor_get_pos() + STEP_EQUALITY_ERROR){	//straight and not circular
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

/*
 * Name: 		-displacement_rotation_angle_check
 * Description:	-checks if the angle given in parameters has been reached or excelled
 * Arguments:	-int16_t angle_degrees: angle that has to be checked [deg]
 * Return:		-boolean (CAUTION: 1 if not reached, 0 if reached)
 *
 * */
uint8_t displacement_rotation_angle_check(int16_t angle_degrees){
	switch(angle_degrees){
	case ANGLE_120_DEGREES:
		if((right_motor_get_pos()-left_motor_get_pos()) >= ANGLE_120_STEP) return ANGLE_REACHED;
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

/*
 * Name: 		-displacement_distance_reset
 * Description:	-resets the static variable containing the distance of the
 * 				wheels (and hence the angle of the robot) to 0
 * Arguments:	-void
 * Return:		-void
 *
 * */
void displacement_distance_reset(void){
	left_motor_set_pos(RESET_LEFT_POS_MOTOR);
	right_motor_set_pos(RESET_RIGHT_POS_MOTOR);
}

/*
 * Name: 		-displacement_angle_reset
 * Description:	-resets the static variable containing the distance of the
 * 				wheels to 0
 * Arguments:	-void
 * Return:		-void
 *
 * */
void displacement_angle_reset(void){
	left_motor_set_pos(RESET_LEFT_POS_MOTOR);
	right_motor_set_pos(RESET_RIGHT_POS_MOTOR);
}

/*
 * Name: 		-displacement_rotation
 * Description:	-sets the wheels' speeds in order to obtain a rotation
 * Arguments:	-int32_t speed: the speed of the wheels for rotation [step]
 * Return:		-void
 *
 * */
void displacement_rotation(int32_t speed)
{
	if(speed == NORMAL_ROT_SPEED){
		right_motor_set_speed(NORMAL_ROT_SPEED);
		left_motor_set_speed(-NORMAL_ROT_SPEED);
	}
	else{
		left_motor_set_speed(NO_SPEED);
		right_motor_set_speed(NO_SPEED);
	}
}

/*
 * Name: 		-displacement_straight_speed_set
 * Description:	-sets the wheels' speeds in order to obtain a straight trajectory
 * Arguments:	-int32_t speed: the speed of the wheels [step]
 * Return:		-void
 *
 * */
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

/*
 * Name: 		-displacement_circle_speed
 * Description:	-sets the wheels' speeds in order to obtain a circular trajectory with maximum speed
 * Arguments:	-void
 * Return:		-void
 *
 * */
void displacement_circle_speed(void){
	switch(figure_size_get()){
	case FIGURE_NO_SIZE:{
		right_motor_set_speed(NO_SPEED);
		left_motor_set_speed(NO_SPEED);
		break;
	}
	case FIGURE_SIZE_1:{
		left_motor_set_speed(CIRCLE_SPEED_LEFT_1);
		right_motor_set_speed(CIRCLE_SPEED_RIGHT_1);
		break;
	}
	case FIGURE_SIZE_2:{
		left_motor_set_speed(CIRCLE_SPEED_LEFT_2);
		right_motor_set_speed(CIRCLE_SPEED_RIGHT_2);
		break;
	}
	case FIGURE_SIZE_MAX:{
		left_motor_set_speed(CIRCLE_SPEED_LEFT_MAX);
		right_motor_set_speed(CIRCLE_SPEED_RIGHT_MAX);
		break;
	}
	default:{
		left_motor_set_speed(NO_SPEED);
		right_motor_set_speed(NO_SPEED);
	}
	}
}
