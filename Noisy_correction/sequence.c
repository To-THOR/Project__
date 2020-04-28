
//-----------------Sequence of tasks that the robot is going to execute depending on current mode -----------------
// PRIORITIES EXPLANATION: once in MODE #i the robot continues its tasks regardless of changes in MODE #i-1

#include <mode.h>
#include <audio/microphone.h>
#include <audio_processing.h>
#include <fft.h>
#include <figure.h>
#include <displacement.h>
#include <sensors/VL53L0X/VL53L0X.h>
#include <motors.h>
#include <sequence.h>
#include <math.h>
#include <displacement.h>


#define THREAD_SEQ_SIZE 256
#define NB_MEASUREMENTS  29
#define SEQU_THD_PRIO 	NORMALPRIO+1
#define PERIOD_MODE_1	100
#define PERIOD_MODE_2	200 		//to be modified according to the motors speed
#define PERIOD_MODE_3	100


static THD_WORKING_AREA(waSEQUENCE, THREAD_SEQ_SIZE);
static THD_FUNCTION(SEQThd, arg)
{
	while(1)
	{
		static int i = 0;

		//RESET THE FIGURE AND FIGURE SIZE AND ELSE?
		figure_set(FIGURE_NONE);
		figure_size_set(FIGURE_NO_SIZE);

		while (mode_get()== MODE_IDLE)
		// Working principle:
		// microphone is activated, if a sound is detected its frequency refers to a figure (form and dimensions)
		{
			// mic_start => processAudioData => sound_remote
			if (figure_get() != FIGURE_NONE)
				mode_update();
			chThdSleepMilliseconds(PERIOD_MODE_1);
		}

		displacement_angle_reset();
		displacement_rotation(NORMAL_ROT_SPEED);
		while (mode_get()== MODE_CHECK)
		// Working principle:
		// robot spins 360� around itself until distance measurements are done
		// If the distance given by sensor VL53L0X is bigger then the size of the wanted figure: go to mode 3
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
			displacement_distance_reset();
			displacement_straight_speed_set(NORMAL_SPEED);
			while (displacement_straight_distance_check(figure_size_get()))
				chThdSleepMilliseconds(PERIOD_MODE_3);
			displacement_straight_speed_set(NO_SPEED);

			if (figure_get()== FIGURE_CIRCLE)
			{
					//rotate to orient the robot
					displacement_angle_reset();
					displacement_rotation(NORMAL_ROT_SPEED);
					while (displacement_rotation_angle_check(ANGLE_90_DEGREES))
						chThdSleepMilliseconds(PERIOD_MODE_3);
					displacement_rotation(NO_SPEED);
					//draw the figure
					displacement_angle_reset();
					displacement_circle_speed();
					while (displacement_rotation_angle_check(ANGLE_360_DEGREES))
						chThdSleepMilliseconds(PERIOD_MODE_3);
					displacement_rotation(NO_SPEED);
					//rotate to re-orient the robot in order to go back to center
					displacement_angle_reset();
					displacement_rotation(NORMAL_ROT_SPEED);
					while (displacement_rotation_angle_check(ANGLE_90_DEGREES))
						chThdSleepMilliseconds(PERIOD_MODE_3);
					displacement_rotation(NO_SPEED);
			}

			if (figure_get()== FIGURE_SQUARE)
			{
					 // rotate to orient robot
					 displacement_angle_reset();
			  		 displacement_rotation(NORMAL_ROT_SPEED);
			  		 while (displacement_rotation_angle_check(ANGLE_SQUARE_DEGREES))
			  			chThdSleepMilliseconds(PERIOD_MODE_3);
			  		displacement_rotation(NO_SPEED);
			  		// draw figure
			  		for (i=0; i<3; i++)
			  		{
			  			displacement_distance_reset();
			  			displacement_straight_speed_set(NORMAL_SPEED);
			  			while (displacement_straight_distance_check(figure_side_get()))
			  				chThdSleepMilliseconds(PERIOD_MODE_3);
			  			displacement_straight_speed_set(NO_SPEED);
			  			displacement_angle_reset();
			  			displacement_rotation(NORMAL_ROT_SPEED);
			  			while (displacement_rotation_angle_check(ANGLE_90_DEGREES))
			  				 chThdSleepMilliseconds(PERIOD_MODE_3);
			  			displacement_rotation(NO_SPEED);
			  		}
			  		displacement_distance_reset();
			  		displacement_straight_speed_set(NORMAL_SPEED);
			  		while (displacement_straight_distance_check(figure_side_get()))
			  			chThdSleepMilliseconds(PERIOD_MODE_3);
			  		displacement_straight_speed_set(NO_SPEED);
			  		//rotate to re-orient the robot in order to go back to center
			  		displacement_angle_reset();
			  		displacement_rotation(NORMAL_ROT_SPEED);
			  		while (displacement_rotation_angle_check(ANGLE_SQUARE_DEGREES))
			  			chThdSleepMilliseconds(PERIOD_MODE_3);
			  		displacement_rotation(NO_SPEED);
			}

			if (figure_get()== FIGURE_TRIANGLE)
			{
				// rotate to orient robot
				displacement_angle_reset();
				displacement_rotation(NORMAL_ROT_SPEED);
				while (displacement_rotation_angle_check(ANGLE_TRIANGLE_DEGREES))
					chThdSleepMilliseconds(PERIOD_MODE_3);
				displacement_rotation(NO_SPEED);
				for (i=0; i<2; i++)
				{
					displacement_distance_reset();
					displacement_straight_speed_set(NORMAL_SPEED);
					while (displacement_straight_distance_check(figure_side_get()))
						chThdSleepMilliseconds(PERIOD_MODE_3);
					displacement_straight_speed_set(NO_SPEED);
					displacement_angle_reset();
					displacement_rotation(NORMAL_ROT_SPEED);
					while (displacement_rotation_angle_check(ANGLE_120_DEGREES))
						chThdSleepMilliseconds(PERIOD_MODE_3);
					displacement_rotation(NO_SPEED);
				}
				displacement_distance_reset();
				displacement_straight_speed_set(NORMAL_SPEED);
				while (displacement_straight_distance_check(figure_side_get()))
					chThdSleepMilliseconds(PERIOD_MODE_3);
				displacement_straight_speed_set(NO_SPEED);
				//rotate to re-orient the robot in order to go back to center
				displacement_angle_reset();
				displacement_rotation(NORMAL_ROT_SPEED);
				while (displacement_rotation_angle_check(ANGLE_TRIANGLE_DEGREES))
					chThdSleepMilliseconds(PERIOD_MODE_3);
				displacement_rotation(NO_SPEED);
			}


			//return from starting point to center
			displacement_distance_reset();
			displacement_straight_speed_set(NORMAL_SPEED);
			while (displacement_straight_distance_check(figure_size_get()))
				chThdSleepMilliseconds(PERIOD_MODE_3);
			displacement_straight_speed_set(NO_SPEED);

			mode_update();
		}
	}
}

void sequence_sequthd_init(void){
	(void)chThdCreateStatic(waSEQUENCE, sizeof(waSEQUENCE),
	    SEQU_THD_PRIO, SEQThd, NULL);
}
