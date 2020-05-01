// --------------------------- DESCRIPTION ---------------------------
//displacement.h
//file responsible for the distance and speed processing

#ifndef DISPLACEMENT_H_
#define DISPLACEMENT_H_

// --------------------------- INCLUDES ---------------------------

#include <figure.h>
#include <motors.h>

// --------------------------- CONSTANTS ---------------------------

#define NO_SPEED					0
#define NORMAL_SPEED				MOTOR_SPEED_LIMIT		//[step/sec]
#define NORMAL_ROT_SPEED 			400						//[step/sec]
#define ANGLE_120_DEGREES			120						// angle = 120[deg]
#define ANGLE_90_DEGREES			90						// angle = 90 [deg]
#define ANGLE_SQUARE_DEGREES		135						// angle = 135[deg]
#define ANGLE_TRIANGLE_DEGREES		150						// angle = 150[deg]
#define ANGLE_360_DEGREES			360						// angle = 360[deg]
#define ANGLE_REACHED				(uint8_t)(0)
#define ANGLE_NOT_REACHED			(uint8_t)(1)
#define DISTANCE_REACHED			(uint8_t)(0)
#define	DISTANCE_NOT_REACHED		(uint8_t)(1)


// --------------------------- FUNCTIONS ---------------------------

/*
 * Name: 		-displacement_start
 * Description:	-initializes the threads related to displacement
 * Arguments:	-void
 * Return:		-void
 *
 * */
void 		displacement_start(void);

/*
 * Name: 		-displacement_straight_distance_check
 * Description:	-checks if the distance given in parameters has been reached or excelled
 * 				-CAUTION: maximum distance is 2^16[step] = 8.5 [m] due to the motor structure implementation in motors.h
 * Arguments:	-int16_t distance_mm: distance that has to be checked [mm]
 * Return:		-boolean (CAUTION: 1 if not reached, 0 if reached)
 *
 * */
uint8_t 	displacement_straight_distance_check(int16_t distance_mm);

/*
 * Name: 		-displacement_rotation_angle_check
 * Description:	-checks if the angle given in parameters has been reached or excelled
 * Arguments:	-int16_t angle_degrees: angle that has to be checked [deg]
 * Return:		-boolean (CAUTION: 1 if not reached, 0 if reached)
 *
 * */
uint8_t displacement_rotation_angle_check(int16_t angle_degrees);

/*
 * Name: 		-displacement_distance_reset
 * Description:	-resets the static variable containing the distance of the
 * 				wheels (and hence the angle of the robot) to 0
 * Arguments:	-void
 * Return:		-void
 *
 * */
void 		displacement_distance_reset(void);

/*
 * Name: 		-displacement_angle_reset
 * Description:	-resets the static variable containing the distance of the
 * 				wheels to 0
 * Arguments:	-void
 * Return:		-void
 *
 * */
void 		displacement_angle_reset(void);

/*
 * Name: 		-displacement_rotation
 * Description:	-sets the wheels' speeds in order to obtain a rotation
 * Arguments:	-int32_t speed: the speed of the wheels for rotation [step]
 * Return:		-void
 *
 * */
void 		displacement_rotation(int32_t speed);

/*
 * Name: 		-displacement_straight_speed_set
 * Description:	-sets the wheels' speeds in order to obtain a straight trajectory
 * Arguments:	-int32_t speed: the speed of the wheels [step]
 * Return:		-void
 *
 * */
void 		displacement_straight_speed_set(int32_t speed);

/*
 * Name: 		-displacement_circle_speed
 * Description:	-sets the wheels' speeds in order to obtain a circular trajectory with maximum speed
 * Arguments:	-void
 * Return:		-void
 *
 * */
void 		displacement_circle_speed(void);

#endif /* DISPLACEMENT_H_ */
