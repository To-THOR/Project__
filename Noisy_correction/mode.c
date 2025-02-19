// --------------------------- DESCRIPTION ---------------------------
//figure.c
//file responsible for the mode implementation, using static variables

// --------------------------- INCLUDES ---------------------------

#include "ch.h"
#include "hal.h"
#include <main.h>
#include <mode.h>

// --------------------------- CONSTANTS ---------------------------

#define NO_ERROR	0
#define ERROR		1

// --------------------------- VARIABLES ---------------------------

/*
 * Name: 		-static_mode
 * Description:	-current mode of the robot
 *
 * */
static int static_mode 	= MODE_IDLE;

/*
 * Name: 		-static_error
 * Description:	-error variable corresponding to the presence of an obstacle
 *
 * */
static int static_error	= NO_ERROR;

// --------------------------- FUNCTIONS ---------------------------

/*
 * Name: 		-mode_update
 * Description:	-updates the mode static variable to the next state
 * Arguments:	-void
 * Return:		-void
 *
 * */
void mode_update(void){
	switch(static_mode){
	case MODE_IDLE:{
		static_mode = MODE_CHECK;
		break;
	}
	case MODE_CHECK:{
		if(static_error == ERROR){
			static_mode  = MODE_IDLE;
			static_error = NO_ERROR;
		}
		else static_mode = MODE_DRAW;
		break;
	}
	case MODE_DRAW:{
		static_mode = MODE_IDLE;
		break;
	}
	default:{
		static_error = NO_ERROR;
		static_mode  = MODE_IDLE;
	}
	}

	return;
}

/*
 * Name: 		-mode_raise_error
 * Description:	-indicates that there is an obstacle (by changing static_error)
 * Arguments:	-void
 * Return:		-void
 *
 * */
void mode_raise_error(void){
	static_error = ERROR;
	return;
}

/*
 * Name: 		-mode_get
 * Description:	-Returns the value of static_mode
 * Arguments:	-void
 * Return:		-static_mode
 *
 * */
int mode_get(void){
	return static_mode;
}
