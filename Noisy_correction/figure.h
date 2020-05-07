// --------------------------- DESCRIPTION ---------------------------
//figure.h
//file responsible for the geometric shape and size implementation, using static variables

#ifndef FIGURE_H_
#define FIGURE_H_

// --------------------------- INCLUDES ---------------------------

#include <sys\_stdint.h>

// --------------------------- CONSTANTS ---------------------------

#define FIGURE_NONE		0
#define FIGURE_SQUARE	1
#define	FIGURE_TRIANGLE	2
#define	FIGURE_CIRCLE	3

#define	FIGURE_NO_SIZE	(uint16_t)(0)	//[mm]
#define FIGURE_SIZE_1	(uint16_t)(70) //[mm]
#define FIGURE_SIZE_2	(uint16_t)(100)	//[mm]
#define FIGURE_SIZE_MAX	(uint16_t)(150)	//[mm]

#define SQUARE_FACTOR		(float)(1.4142)	//sqrt(2)
#define TRIANGLE_FACTOR		(float)(1.7321)	//sqrt(3)

#define SIDE_ZERO			0
#define SQUARE_SIDE_1		(int16_t)(SQUARE_FACTOR*FIGURE_SIZE_1)		//[mm]
#define SQUARE_SIDE_2		(int16_t)(SQUARE_FACTOR*FIGURE_SIZE_2)		//[mm]
#define SQUARE_SIDE_MAX		(int16_t)(SQUARE_FACTOR*FIGURE_SIZE_MAX)	//[mm]
#define TRIANGLE_SIDE_1		(int16_t)(TRIANGLE_FACTOR*FIGURE_SIZE_1)	//[mm]
#define TRIANGLE_SIDE_2		(int16_t)(TRIANGLE_FACTOR*FIGURE_SIZE_2)	//[mm]
#define TRIANGLE_SIDE_MAX	(int16_t)(TRIANGLE_FACTOR*FIGURE_SIZE_MAX)	//[mm]

// --------------------------- FUNCTIONS ---------------------------

/*
 * Name: 		-figure_set
 * Description:	-Modifies the value of figure_static
 * Arguments:	-int figure_type: new value for figure_static
 * Return:		-void
 *
 * */
void figure_set(int figure_type);

/*
 * Name: 		-figure_get
 * Description:	-Returns the value of figure_static
 * Arguments:	-void
 * Return:		-figure_static
 *
 * */
int8_t figure_get(void);

/*
 * Name: 		-figure_size_get
 * Description:	-Returns the value of figure_size_static
 * Arguments:	-void
 * Return:		-figure_size_static
 *
 * */
int16_t figure_size_get(void);

/*
 * Name: 		-figure_side_get
 * Description:	-Returns the value of the side of the geometric shape currently drawn
 * Arguments:	-void
 * Return:		-side of the shape
 *
 * */
int16_t figure_side_get(void);

/*
 * Name: 		-figure_size_set
 * Description:	-Modifies the value of figure_size_static
 * Arguments:	-int figure_size: new value for figure_size_static
 * Return:		-void
 *
 * */
void figure_size_set(int figure_size);

#endif /* FIGURE_H_ */
