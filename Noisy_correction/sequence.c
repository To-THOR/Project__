
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


#define THREAD_SEQ_SIZE 256
#define SPEED 500


static THD_WORKING_AREA(waSEQUENCE, THREAD_SEQ_SIZE);
static THD_FUNCTION(SEQThd, arg)
{
	while (mode_get()== MODE_IDLE)
	// Working principle:
	// microphone is activated, if a sound is detected its frequency refers to a figure (form and dimensions)
	{

	}
	while (mode_get()== MODE_CHECK)
	// Working principle:
	// robot spins 360° around itself until distance measurements are done
	// If the distance given by sensor VL53L0X is bigger then the size of the wanted figure: go to mode 3
	// If the distance is smaller: error: go to mode 1
	{
		right_motor_set_speed(SPEED);
		left_motor_set_speed(-SPEED);
		int dist = (int) VL53L0X_get_dist_mm(); // initially an uint16_t
		int i= i+1; // incrementation of number of measurements counter
		if(dist <= figure_size_get())
			mode_raise_error();
		if(i= 29) // number of needed measurements, dep on chosen frequency
			mode_update();
	}
	while (mode_get()== MODE_DRAW)
	{

	}

	chThdSleepMilliseconds(200);
}
