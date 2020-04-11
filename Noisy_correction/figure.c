#include "ch.h"
#include "hal.h"
#include <main.h>
#include <figure.h>

// --------------------------- VARIABLES ---------------------------

/*
 * Name: 		-figure_static
 * Description:	-figure currently being drawn by the robot
 *
 * */
static int figure_static = FIGURE_NONE;


// --------------------------- FUNCTIONS ---------------------------

/*
 * Name: 		-figure_set
 * Description:	-Modifies the value of figure_static
 * Arguments:	-int figure_type: new value for figure_static
 * Return:		-void
 *
 * */
void figure_set(int figure_type){
	if(figure_type <= FIGURE_CIRCLE && figure_type >= FIGURE_SQUARE){
		figure_static = figure_type;
	}
	else{
		figure_static = FIGURE_NONE;
	}

	return;
}

/*
 * Name: 		-figure_get
 * Description:	-Returns the value of figure_static
 * Arguments:	-void
 * Return:		-figure_static
 *
 * */
int figure_get(void){
	return figure_static;
}
