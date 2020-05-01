// --------------------------- DESCRIPTION ---------------------------
//sequence.c
//Sequence of tasks that the robot is going to execute depending on current mode.
//Once in MODE #i the robot continues its tasks regardless of changes in MODE #i-1

// --------------------------- INCLUDES ---------------------------

#include <mode.h>
#include <figure.h>
#include <displacement.h>
#include <sensors/VL53L0X/VL53L0X.h>
#include <sequence.h>
#include <math.h>
#include <displacement.h>
#include<audio_processing.h>

// --------------------------- CONSTANTS ---------------------------

#define NB_MEASUREMENTS  29
#define PERIOD_MODE_1	100	// [ms]
#define PERIOD_MODE_2	200	// [ms]
#define PERIOD_MODE_3	100	// [ms]

// --------------------------- FUNCTIONS ---------------------------

/*
 * Name: 		-sequence_go_straight
 * Description:	-controls the robot in order to reach straight distance given in parameter
 * Arguments:	-uint32_t speed: speed of the robot [step/s]
 * 				-int16_t distance: distance that has to be reached [mm]
 * Return:		-void
 *
 * */
void sequence_go_straight(uint32_t speed,int16_t distance);

/*
 * Name: 		-sequence_go_rotation
 * Description:	-controls the robot in order to rotate and reach an angle given in parameter
 * Arguments:	-uint32_t speed: speed of the robot [step/s]
 * 				-int16_t angle: angle that has to be reached [deg]
 * Return:		-void
 *
 * */
void sequence_go_rotation(uint32_t speed,int16_t angle);

/*
 * Name: 		-sequence_go_circle
 * Description:	-controls the robot in order to draw a circle
 * Arguments:	-void
 * Return:		-void
 *
 * */
void sequence_go_circle(void);

/*
 * Name: 		-sequence_main_function
 * Description:	-principal function of the system, containing the three finite loops for the three modes per cycle
 * Arguments:	-void
 * Return:		-void
 *
 * */
void sequence_main_function(void){
	static int i = 0;

	max_norm_buff_reset();
	figure_set(FIGURE_NONE);

	while (mode_get()== MODE_IDLE)
		// Working principle:
		// microphone is activated, if a sound is detected its frequency refers to a figure (form and dimensions)
	{
		// mic_start => processAudioData => sound_remote
		if (figure_get() != FIGURE_NONE && figure_size_get() != FIGURE_NO_SIZE)
			mode_update();
		chThdSleepMilliseconds(PERIOD_MODE_1);
	}

	displacement_angle_reset();
	displacement_rotation(NORMAL_ROT_SPEED);
	while (mode_get()== MODE_CHECK)
		// Working principle:
		// robot spins 360deg around itself until distance measurements are done
		// If the distance given by sensor VL53L0X is bigger than the size of the wanted figure: go to mode 3
		// If the distance is smaller: error: go to mode 1
	{
		if((int) VL53L0X_get_dist_mm() <= figure_size_get()){ //distance is initially an uint16_t
			mode_raise_error();
			mode_update();
		}
		if(!displacement_rotation_angle_check(ANGLE_360_DEGREES)) // number of needed measurements, dep on chosen frequency
			mode_update();

		chThdSleepMilliseconds(PERIOD_MODE_2);
	}

	displacement_rotation(NO_SPEED);

	if (mode_get()== MODE_DRAW)
		// Working principle:
		// robot moves vertically with a distance = radius of the circumscribed circle for the start position
		// robot moves according to  the type of the chosen figure
	{
		// move from center to starting point
		sequence_go_straight(NORMAL_SPEED,figure_size_get());

		if (figure_get()== FIGURE_CIRCLE)
		{
			//rotate to orient the robot
			sequence_go_rotation(NORMAL_ROT_SPEED, ANGLE_90_DEGREES);
			//draw the figure
			sequence_go_circle();
			//rotate to re-orient the robot in order to go back to center
			sequence_go_rotation(NORMAL_ROT_SPEED, ANGLE_90_DEGREES);
		}

		if (figure_get()== FIGURE_SQUARE)
		{
			// rotate to orient robot
			sequence_go_rotation(NORMAL_ROT_SPEED, ANGLE_SQUARE_DEGREES);
			// draw figure
			for (i=0; i<3; i++)
			{
				sequence_go_straight(NORMAL_SPEED,figure_side_get());
				sequence_go_rotation(NORMAL_ROT_SPEED, ANGLE_90_DEGREES);
			}
			sequence_go_straight(NORMAL_SPEED,figure_side_get());
			//rotate to re-orient the robot in order to go back to center
			sequence_go_rotation(NORMAL_ROT_SPEED, ANGLE_SQUARE_DEGREES);
		}

		if (figure_get()== FIGURE_TRIANGLE)
		{
			// rotate to orient robot
			sequence_go_rotation(NORMAL_ROT_SPEED, ANGLE_TRIANGLE_DEGREES);
			for (i=0; i<2; i++)
			{
				sequence_go_straight(NORMAL_SPEED,figure_side_get());
				sequence_go_rotation(NORMAL_ROT_SPEED, ANGLE_120_DEGREES);
			}
			sequence_go_straight(NORMAL_SPEED,figure_side_get());
			//rotate to re-orient the robot in order to go back to center
			sequence_go_rotation(NORMAL_ROT_SPEED, ANGLE_TRIANGLE_DEGREES);
		}

		//return from starting point to center
		sequence_go_straight(NORMAL_SPEED,figure_size_get());

		mode_update();
	}
}

/*
 * Name: 		-sequence_initial_reset
 * Description:	-resets the necessary variables before entering the infinite loop
 * Arguments:	-void
 * Return:		-void
 *
 * */
void sequence_initial_reset(void){
	figure_size_set(FIGURE_NO_SIZE);
	max_norm_buff_reset();
	figure_set(FIGURE_NONE);
	return;
}

/*
 * Name: 		-sequence_go_straight
 * Description:	-controls the robot in order to reach straight distance given in parameter
 * Arguments:	-uint32_t speed: speed of the robot [step/s]
 * 				-int16_t distance: distance that has to be reached [mm]
 * Return:		-void
 *
 * */
void sequence_go_straight(uint32_t speed,int16_t distance){
	displacement_distance_reset();
	displacement_straight_speed_set(speed);
	while (displacement_straight_distance_check(distance))
		chThdSleepMilliseconds(PERIOD_MODE_3);
	displacement_straight_speed_set(NO_SPEED);
}

/*
 * Name: 		-sequence_go_rotation
 * Description:	-controls the robot in order to rotate and reach an angle given in parameter
 * Arguments:	-uint32_t speed: speed of the robot [step/s]
 * 				-int16_t angle: angle that has to be reached [deg]
 * Return:		-void
 *
 * */
void sequence_go_rotation(uint32_t speed,int16_t angle){
	displacement_angle_reset();
	displacement_rotation(speed);
	while (displacement_rotation_angle_check(angle))
		chThdSleepMilliseconds(PERIOD_MODE_3);
	displacement_rotation(NO_SPEED);
}

/*
 * Name: 		-sequence_go_circle
 * Description:	-controls the robot in order to draw a circle
 * Arguments:	-void
 * Return:		-void
 *
 * */
void sequence_go_circle(void){
	displacement_angle_reset();
	displacement_circle_speed();
	while (displacement_rotation_angle_check(ANGLE_360_DEGREES))
		chThdSleepMilliseconds(PERIOD_MODE_3);
	displacement_rotation(NO_SPEED);
}
