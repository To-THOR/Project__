#ifndef FIGURE_H_
#define FIGURE_H_

// --------------------------- CONSTANTS ---------------------------

#define FIGURE_NONE		0
#define FIGURE_SQUARE	1
#define	FIGURE_TRIANGLE	2
#define	FIGURE_CIRCLE	3

#define	FIGURE_NO_SIZE	(uint16_t)(0)	//[mm]
#define FIGURE_SIZE_1	(uint16_t)(70) //[mm] ////////////////////////////////////for debug
#define FIGURE_SIZE_2	(uint16_t)(100)	//[mm]
#define FIGURE_SIZE_MAX	(uint16_t)(150)	//[mm]

#define SQUARE_FACTOR		(float)(1.4142)
#define TRIANGLE_FACTOR		(float)(1.7321)

#define SIDE_ZERO			0
#define SQUARE_SIDE_1		(int16_t)(SQUARE_FACTOR*FIGURE_SIZE_1)		//[mm]
#define SQUARE_SIDE_2		(int16_t)(SQUARE_FACTOR*FIGURE_SIZE_2)
#define SQUARE_SIDE_MAX		(int16_t)(SQUARE_FACTOR*FIGURE_SIZE_MAX)
#define TRIANGLE_SIDE_1		(int16_t)(TRIANGLE_FACTOR*FIGURE_SIZE_1)
#define TRIANGLE_SIDE_2		(int16_t)(TRIANGLE_FACTOR*FIGURE_SIZE_2)
#define TRIANGLE_SIDE_MAX	(int16_t)(TRIANGLE_FACTOR*FIGURE_SIZE_MAX)

// --------------------------- FUNCTIONS ---------------------------

void figure_set(int figure_type);
int8_t figure_get(void);
int16_t figure_size_get(void);
int16_t figure_side_get(void);
void figure_size_set(int figure_size);

#endif /* FIGURE_H_ */
