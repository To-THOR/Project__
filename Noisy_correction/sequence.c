
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
#define NB_MEASUREMENTS  29
#define NORMAL_SPEED 	500
#define SPEED_NULL 		  0
#define SEQU_THD_PRIO 	NORMALPRIO+1
#define PERIOD_MODE_1	100
#define PERIOD_MODE_2	200 //to be modified according to the motors speed
#define CONVERT			10  //to convert units from [0.1mm] to [mm]



static THD_WORKING_AREA(waSEQUENCE, THREAD_SEQ_SIZE);
static THD_FUNCTION(SEQThd, arg)
{
	while(1)
	{

		static int i = 0;
		while (mode_get()== MODE_IDLE)
		// Working principle:
		// microphone is activated, if a sound is detected its frequency refers to a figure (form and dimensions)
		{
			// mic_start => processAudioData => sound_remote
			if (figure_get()> FIGURE_NONE)
				mode_update();
			chThdSleepMilliseconds(PERIOD_MODE_1);
		}

		i = 0;
		while (mode_get()== MODE_CHECK)
		// Working principle:
		// robot spins 360° around itself until distance measurements are done
		// If the distance given by sensor VL53L0X is bigger then the size of the wanted figure: go to mode 3
		// If the distance is smaller: error: go to mode 1
		{
			displacement_rotation(NORMAL_SPEED);
			i= i+1; // incrementation of number of measurements counter
			if((int) VL53L0X_get_dist_mm() <= figure_size_get()) //distance is initially an uint16_t
				mode_raise_error();
			if(i == NB_MEASUREMENTS) // number of needed measurements, dep on chosen frequency
				mode_update();

			chThdSleepMilliseconds(PERIOD_MODE_2);
		}

		if (mode_get()== MODE_DRAW)
		// Working principle:
		// robot moves vertically with a distance = radius of the circumscribed circle for the start position
		// robot moves according to  the type of the chosen figure
		{
			// robot draws figure
			if (figure_get()== FIGURE_CIRCLE)
			{
				while(1)
				{
					displacement_starting_point(void);
					//fct pour vitesse dans displacement
				}
			}
			/*if (figure_get()== FIGURE_SQUARE)
			  	 while(1){
			  	  displacement_starting_point(void);
			  	  }
			if (figure_get()== FIGURE_TRIANGLE)
				while(1){
				displacement_starting_point(void);
				}
			*/
		}
	}
}

void sequence_sequthd_init(void){
	(void)chThdCreateStatic(waSEQUENCE, sizeof(waSEQUENCE),
	    SEQU_THD_PRIO, SEQThd, NULL);
}
