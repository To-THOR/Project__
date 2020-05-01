// --------------------------- DESCRIPTION ---------------------------
//main.c
//main file

// --------------------------- INCLUDES ---------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ch.h"
#include "hal.h"
#include "memory_protection.h"
#include <main.h>
#include <audio/microphone.h>
#include <audio_processing.h>
#include <displacement.h>
#include <sensors/VL53L0X/VL53L0X.h>
#include <sequence.h>

// --------------------------- CONSTANTS ---------------------------

#define PERIOD_MAIN	1000	// [ms]

// --------------------------- FUNCTIONS ---------------------------

/*
 * Name: 		-main
 * Description:	-main function
 * Arguments:	-void
 * Return:		-int
 *
 * */
int main(void)
{

    halInit();
    chSysInit();
    mpu_init();

    //initialize the distance sensor
    VL53L0X_start();

    //initialize the displacement threads
    displacement_start();

    //starts the microphones processing thread.
    //it calls the callback given in parameter when samples are ready
    // sound remote will then set the figure when in MODE_IDLE
    mic_start(&processAudioData);

    sequence_initial_reset();

    /* Infinite loop. */
    while (1) {
    	sequence_main_function();
    }
}

#define STACK_CHK_GUARD 0xe2dee396
uintptr_t __stack_chk_guard = STACK_CHK_GUARD;

void __stack_chk_fail(void)
{
    chSysHalt("Stack smashing detected");
}
