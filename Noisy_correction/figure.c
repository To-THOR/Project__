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
static int8_t figure_static = FIGURE_NONE;

/*
 * Name: 		-figure_size_static
 * Description:	-size of the figure currently being drawn by the robot (radius of the circle of operation [mm])
 *
 * */
static int16_t figure_size_static = FIGURE_NO_SIZE;


// --------------------------- FUNCTIONS ---------------------------

/*
 * Name: 		-figure_set
 * Description:	-Modifies the value of figure_static
 * Arguments:	-int figure_type: new value for figure_static
 * Return:		-void
 *
 * */
void figure_set(int figure_type){
	if(figure_type == FIGURE_NONE || figure_type == FIGURE_CIRCLE \
			|| figure_type == FIGURE_TRIANGLE || figure_type == FIGURE_SQUARE){	//Checking the validity of the
																				//parameter
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
int8_t figure_get(void){
	return figure_static;
}

/*
 * Name: 		-figure_size_get
 * Description:	-Returns the value of figure_size_static
 * Arguments:	-void
 * Return:		-figure_size_static
 *
 * */
int16_t figure_size_get(void){
	return figure_size_static;
}

int16_t figure_side_get(void){
	switch(figure_static){
	case FIGURE_NONE:
		return	SIDE_ZERO;
		break;
	case FIGURE_CIRCLE:
		return figure_size_static;
		break;
	case FIGURE_SQUARE:
		switch (figure_size_static){
		case FIGURE_NO_SIZE:
			return SIDE_ZERO;
			break;
		case FIGURE_SIZE_1:
			return SQUARE_SIDE_1;
			break;
		case FIGURE_SIZE_2:
			return SQUARE_SIDE_2;
			break;
		case FIGURE_SIZE_MAX:
			return SQUARE_SIDE_MAX;
			break;
		default:
			return SIDE_ZERO;;
		}
		break;
	case FIGURE_TRIANGLE:
		switch (figure_size_static){
		case FIGURE_NO_SIZE:
			return SIDE_ZERO;
			break;
		case FIGURE_SIZE_1:
			return TRIANGLE_SIDE_1;
			break;
		case FIGURE_SIZE_2:
			return TRIANGLE_SIDE_2;
			break;
		case FIGURE_SIZE_MAX:
			return TRIANGLE_SIDE_MAX;
			break;
		default:
			return SIDE_ZERO;
		}
		break;
	default:
		return SIDE_ZERO;
	}
}

/*
 * Name: 		-figure_size_set
 * Description:	-Modifies the value of figure_size_static
 * Arguments:	-int figure_size: new value for figure_size_static
 * Return:		-void
 *
 * */
void figure_size_set(int figure_size){
	if(figure_size == FIGURE_NO_SIZE ||figure_size == FIGURE_SIZE_1 || figure_size == FIGURE_SIZE_2 || figure_size == FIGURE_SIZE_MAX){
			figure_size_static = figure_size;
		}
		else{
			figure_size = FIGURE_NO_SIZE;
		}

	return;
}


