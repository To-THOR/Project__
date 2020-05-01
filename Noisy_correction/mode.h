// --------------------------- DESCRIPTION ---------------------------
//figure.h
//file responsible for the mode implementation, using static variables

#ifndef MODE_H_
#define MODE_H_

// --------------------------- CONSTANTS ---------------------------

#define MODE_IDLE	0
#define MODE_CHECK	1
#define MODE_DRAW	2

// --------------------------- FUNCTIONS ---------------------------

/*
 * Name: 		-mode_update
 * Description:	-updates the mode static variable to the next state
 * Arguments:	-void
 * Return:		-void
 *
 * */
void mode_update(void);

/*
 * Name: 		-mode_raise_error
 * Description:	-indicates that there is an obstacle (by changing static_error)
 * Arguments:	-void
 * Return:		-void
 *
 * */
void mode_raise_error(void);

/*
 * Name: 		-mode_get
 * Description:	-Returns the value of static_mode
 * Arguments:	-void
 * Return:		-static_mode
 *
 * */
int mode_get(void);

#endif /* MODE_H_ */
