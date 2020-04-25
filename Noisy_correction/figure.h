#ifndef FIGURE_H_
#define FIGURE_H_

// --------------------------- CONSTANTS ---------------------------

#define FIGURE_NONE		0
#define FIGURE_SQUARE	1
#define	FIGURE_TRIANGLE	2
#define	FIGURE_CIRCLE	3

#define	FIGURE_NO_SIZE	(uint16_t)(0)	//[mm]
#define FIGURE_SIZE_1	(uint16_t)200 	//[mm]
#define FIGURE_SIZE_2	(uint16_t)500	//[mm]
#define FIGURE_SIZE_MAX	(uint16_t)800	//[mm]

// --------------------------- FUNCTIONS ---------------------------

void figure_set(int figure_type);
int figure_get(void);
int figure_size_get(void);
void figure_size_set(int figure_size);

#endif /* FIGURE_H_ */
