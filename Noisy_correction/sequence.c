
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


#define THREAD_SEQ_SIZE 256
#define 				SPEED 500
#define SEQU_THD_PRIO 	NORMALPRIO+1
#define PERIOD_MODE_2	200 //to be modified according to the motors speed


static THD_WORKING_AREA(waSEQUENCE, THREAD_SEQ_SIZE);
static THD_FUNCTION(SEQThd, arg)
{
	while(1){

		static int i = 0;
		while (mode_get()== MODE_IDLE)
		// Working principle:
		// microphone is activated, if a sound is detected its frequency refers to a figure (form and dimensions)
		{
			figure_size_set(FIGURE_SIZE_MAX);
			mode_update();
		}

		i = 0;
		while (mode_get() == MODE_CHECK)
		// Working principle:
		// robot spins 360° around itself until distance measurements are done
		// If the distance given by sensor VL53L0X is bigger then the size of the wanted figure: go to mode 3
		// If the distance is smaller: error: go to mode 1
		{
			right_motor_set_speed(0);
			left_motor_set_speed(0);
			i= i+1; // incrementation of number of measurements counter
			if((int) VL53L0X_get_dist_mm() <= figure_size_get()){ //distance is initially an uint16_t
				mode_raise_error();
				right_motor_set_speed(SPEED);
				left_motor_set_speed(-SPEED);
			}
			if(i == 29) // number of needed measurements, dep on chosen frequency
				mode_update();

			chThdSleepMilliseconds(PERIOD_MODE_2);
		}
		while (mode_get()== MODE_DRAW)
		{
			mode_update();
		}
	}
}

void sequence_sequthd_init(void){
	(void)chThdCreateStatic(waSEQUENCE, sizeof(waSEQUENCE),
	    SEQU_THD_PRIO, SEQThd, NULL);
}
