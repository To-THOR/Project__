#include "ch.h"
#include "hal.h"
#include <main.h>
#include <mode.h>
#include <motors.h>
#include <displacement.h>

void distance_update(void);

// --------------------------- CONSTANTS ---------------------------

#define DISTANCE_THD_PRIO			NORMALPRIO + 3
#define DISTANCE_THD_SIZE			256
#define INITIAL_DISTANCE			0
#define INITIAL_SPEED_LEFT			0
#define INITIAL_SPEED_RIGHT			0
#define DISTANCE_THD_TIME			100
#define VELOCITY_CONSTANT_FACTOR	(float)6.5e-2 	// = (wheels radius[dm]*100) / (2*nb of steps for one complete rotation)
													// = (1.3*100)/(2*1000) [100um/msec]
#define DISTANCE_FACTOR				(int)(VELOCITY_CONSTANT_FACTOR * DISTANCE_THD_TIME) //D = V*dT

// --------------------------- VARIABLES ---------------------------

/*
 * Name: 		-static_distance
 * Description:	-distance done by the robot [100um]
 * 				-CAUTION: the maximum distance is 2^32*100um = 4.3e5 [m]
 *
 * */
static	uint32_t static_distance	= INITIAL_DISTANCE;


/*
 * Name: 		-static_speed_left
 * Description:	-speed of the left wheel of the robot [step/sec]
 * 				-CAUTION: the maximum angle is 1100 step/sec
 * 				-CAUTION: the velocity must have zero dozens and units (only 100,200,300,400,... are acceptable)
 *
 * */
static	uint16_t static_speed_left 	= INITIAL_SPEED_LEFT;

/*
 * Name: 		-static_speed_right
 * Description:	-speed of the right wheel of the robot [step/sec]
 * 				-CAUTION: the maximum angle is 1100 step/sec
 * 				-CAUTION: the velocity must have zero dozens and units (only 100,200,300,400,... are acceptable)
 *
 * */
static	uint16_t static_speed_right = INITIAL_SPEED_RIGHT;


// --------------------------- THREADS ---------------------------

/*
 * Name: 		-DistanceThd
 * Description:	-updates and processes the distance done by the robot according to the mode
 *
 * */
static THD_WORKING_AREA(waDistanceThd, DISTANCE_THD_SIZE);
static THD_FUNCTION(DistanceThd, arg) {
	while(1){
		distance_update();
		//distance_check();
	}
	chThdSleepMilliseconds(DISTANCE_THD_TIME);

}

// --------------------------- FUNCTIONS ---------------------------

/*
 * Name: 		-displacement_start
 * Description:	-initializes the threads (motors and distance processing)
 * Arguments:	-void
 * Return:		-void
 *
 * */
void displacement_start(void){
	motors_init();
//	(void)chThdCreateStatic(waDistanceThd, sizeof(waDistanceThd),
//	    DISTANCE_THD_PRIO, DistanceThd, NULL);
}

/*
 * Name: 		-distance_update
 * Description:	-updates the distance done by the robot according to the wheels velocities
 * Arguments:	-void
 * Return:		-void
 *
 * */
void distance_update(void){
	static_distance +=	DISTANCE_FACTOR * (int)((static_speed_left + static_speed_right)/(int)100);
}
