#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ch.h"
#include "hal.h"
#include "memory_protection.h"
#include <usbcfg.h>
#include <main.h>
#include <chprintf.h>
#include <audio/microphone.h>
#include <audio_processing.h>
#include <fft.h>
#include <communications.h>
#include <arm_math.h>
#include <mode.h>
#include <figure.h>
#include <displacement.h>
#include <sensors/VL53L0X/VL53L0X.h>

#define THREAD_OBSTACLE_DETECTION_SIZE 256
#define OBSTACLE_DETECTION_PRIO		NORMALPRIO + 1

static THD_WORKING_AREA(waObstacleDetection, THREAD_OBSTACLE_DETECTION_SIZE);
static THD_FUNCTION(ObstacleDetectionThd, arg) {
	while(1){
		if(mode_get() == MODE_CHECK){						///ANNULER LE THREAD OU BIEN LAISSER CE IF?
			if(VL53L0X_get_dist_mm() <= figure_size_get()){
				mode_raise_error();
			}
			mode_update();
		}
		chThdSleepMilliseconds(200);
	}
}


int main(void)
{

    halInit();
    chSysInit();
    mpu_init();

    (void)chThdCreateStatic(waObstacleDetection, sizeof(waObstacleDetection),
    OBSTACLE_DETECTION_PRIO, ObstacleDetectionThd, NULL);

    //initialize the displacement threads
    displacement_start();

    //starts the microphones processing thread.
    //it calls the callback given in parameter when samples are ready
    mic_start(&processAudioData);

    /* Infinite loop. */
    while (1) {
    	mode_wait();
    	mode_update();
    }
}

#define STACK_CHK_GUARD 0xe2dee396
uintptr_t __stack_chk_guard = STACK_CHK_GUARD;

void __stack_chk_fail(void)
{
    chSysHalt("Stack smashing detected");
}
