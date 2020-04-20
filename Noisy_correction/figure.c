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

/*
 * Name: 		-figure_size_static
 * Description:	-size of the figure currently being drawn by the robot (radius of the circle of operation [mm])
 *
 * */
static int figure_size_static = FIGURE_NO_SIZE;


// --------------------------- FUNCTIONS ---------------------------

/*
 * Name: 		-figure_set
 * Description:	-Modifies the value of figure_static
 * Arguments:	-int figure_type: new value for figure_static
 * Return:		-void
 *
 * */
void figure_set(int figure_type){
	if(figure_type <= FIGURE_CIRCLE && figure_type >= FIGURE_SQUARE){  	//CAREFUL: the list of figures
																		//have to begin and end with
																		//FIGURE_SQUARE and FIGURE_CIRCLE
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

/*
 * Name: 		-figure_size_get
 * Description:	-Returns the value of figure_size_static
 * Arguments:	-void
 * Return:		-figure_size_static
 *
 * */
int figure_size_get(int figure_size){
	return figure_size_static;
}

/*
 * Name: 		-figure_size_set
 * Description:	-Modifies the value of figure_size_static
 * Arguments:	-int figure_size: new value for figure_size_static
 * Return:		-void
 *
 * */
int figure_size_set(void){
	if(figure_size <= FIGURE_SIZE_MAX && figure_size >= FIGURE_SIZE_1){
			figure_size_static = figure_size;
		}
		else{
			figure_size = FIGURE_NO_SIZE;
		}

	return;
}


