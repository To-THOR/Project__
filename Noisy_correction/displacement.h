#ifndef DISPLACEMENT_H_
#define DISPLACEMENT_H_

#define NO_SPEED					0
#define ROT_SPEED 					500
#define NORMAL_SPEED				700 		//CAUTION: speed must have zero dozens and units (e.g. 100, 200, ...)
// --------------------------- FUNCTIONS ---------------------------

void 		displacement_start(void);
uint16_t 	displacement_distance_get(void);
void 		displacement_distance_reset(void);
void 		displacement_rotation(int32_t speed);
void 		displacement_straight_speed_set(int32_t speed);
//void 		displacement_starting_point(void);
void 		displacement_circle_speed(void);

#endif /* DISPLACEMENT_H_ */
