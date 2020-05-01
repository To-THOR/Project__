#ifndef DISPLACEMENT_H_
#define DISPLACEMENT_H_

#define NO_SPEED					0
#define NORMAL_SPEED				MOTOR_SPEED_LIMIT
#define NORMAL_ROT_SPEED 			400
#define ANGLE_120_DEGREES			120						// angle = 120 	deg
#define ANGLE_90_DEGREES			90						// angle = 90 	deg
#define ANGLE_SQUARE_DEGREES		135						// angle = 135 	deg
#define ANGLE_TRIANGLE_DEGREES		150						// angle = 150 	deg
#define ANGLE_360_DEGREES			360						// angle = 360 	deg
#define ANGLE_REACHED				(uint8_t)(0)
#define ANGLE_NOT_REACHED			(uint8_t)(1)
#define DISTANCE_REACHED			(uint8_t)(0)
#define	DISTANCE_NOT_REACHED		(uint8_t)(1)


// --------------------------- FUNCTIONS ---------------------------

void 		displacement_start(void);
uint8_t 	displacement_straight_distance_check(int16_t distance_mm);
uint8_t displacement_rotation_angle_check(int16_t angle_degrees);
void 		displacement_distance_reset(void);
void 		displacement_angle_reset(void);
void 		displacement_rotation(int32_t speed);
void 		displacement_straight_speed_set(int32_t speed);
void 		displacement_circle_speed(void);

#endif /* DISPLACEMENT_H_ */
