#ifndef DISPLACEMENT_H_
#define DISPLACEMENT_H_

#define NO_SPEED					0
#define ROT_SPEED 					500
#define NORMAL_SPEED				700 					//CAUTION: speed must have zero dozens and units (e.g. 100, 200, ...)
#define NORMAL_ROT_SPEED 			500
#define ANGLE_90_DEGREES			90
#define ANGLE_SQUARE_DEGREES		135						// angle = 135°
#define ANGLE_TRIANGLE_DEGREES		150						// angle = 150°
#define	SIDE_SQUARE_FACTOR			(float)1.4142			//square root of 2
#define SIDE_SQUARE_DISTANCE

// --------------------------- FUNCTIONS ---------------------------

void 		displacement_start(void);
uint8_t 	displacement_straight_distance_check(int16_t distance_mm);
void 		displacement_distance_reset(void);
void 		displacement_angle_reset(void);
void 		displacement_rotation(int32_t speed);
void 		displacement_straight_speed_set(int32_t speed);
void 		displacement_circle_speed(void);

#endif /* DISPLACEMENT_H_ */
