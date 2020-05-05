#ifndef AUDIO_PROCESSING_H
#define AUDIO_PROCESSING_H

// --------------------------- CONSTANTS ---------------------------

#define FFT_SIZE 	1024

// --------------------------- STRUCTURES AND ENUMS ---------------------------

typedef enum {
	//2 times FFT_SIZE because these arrays contain complex numbers (real + imaginary)
	LEFT_CMPLX_INPUT = 0,
	RIGHT_CMPLX_INPUT,
	FRONT_CMPLX_INPUT,
	BACK_CMPLX_INPUT,
	//Arrays containing the computed magnitude of the complex numbers
	LEFT_OUTPUT,
	RIGHT_OUTPUT,
	FRONT_OUTPUT,
	BACK_OUTPUT
} BUFFER_NAME_t;

// --------------------------- FUNCTIONS ---------------------------

void processAudioData(int16_t *data, uint16_t num_samples);

/*
 * Name: 		-max_norm_buff_reset
 * Description:	-resets the values of max_norm_table
 * Arguments:	-void
 * Return:		-void
 *
 * */
void max_norm_buff_reset(void);

#endif /* AUDIO_PROCESSING_H */
