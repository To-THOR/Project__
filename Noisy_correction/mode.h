#ifndef MODE_H_
#define MODE_H_

// --------------------------- CONSTANTS ---------------------------

#define MODE_IDLE	0
#define MODE_CHECK	1
#define MODE_DRAW	2

// --------------------------- FUNCTIONS ---------------------------

void mode_update(void);
void mode_raise_error(void);
int mode_get(void);
void mode_wait(void);
void mode_signal(void);

#endif /* MODE_H_ */
