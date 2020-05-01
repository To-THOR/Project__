// --------------------------- DESCRIPTION ---------------------------
//sequence.h
//Sequence of tasks that the robot is going to execute depending on current mode.
//Once in MODE #i the robot continues its tasks regardless of changes in MODE #i-1

#ifndef SEQUENCE_H_
#define SEQUENCE_H_

// --------------------------- FUNCTIONS ---------------------------

/*
 * Name: 		-sequence_main_function
 * Description:	-main function of the system, containing the three finite loops for the three modes per cycle
 * Arguments:	-void
 * Return:		-void
 *
 * */
void sequence_main_function(void);

/*
 * Name: 		-sequence_initial_reset
 * Description:	-resets the necessary variables before entering the infinite loop
 * Arguments:	-void
 * Return:		-void
 *
 * */
void sequence_initial_reset(void);


#endif /* SEQUENCE_H_ */
