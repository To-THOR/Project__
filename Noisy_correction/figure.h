#ifndef FIGURE_H_
#define FIGURE_H_

// --------------------------- CONSTANTS ---------------------------

#define FIGURE_NONE		0
#define FIGURE_SQUARE	1
#define	FIGURE_TRIANGLE	2
#define	FIGURE_CIRCLE	3

#define	FIGURE_NO_SIZE	0
#define FIGURE_SIZE_1	200
#define FIGURE_SIZE_2	500
#define FIGURE_SIZE_MAX	800

// --------------------------- FUNCTIONS ---------------------------

void figure_set(int figure_type);
int figure_get(void);
int figure_size_get(void);
void figure_size_set(int figure_size);

#endif /* FIGURE_H_ */
