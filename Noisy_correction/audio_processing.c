#include "ch.h"
#include "hal.h"
#include <main.h>
#include <usbcfg.h>
#include <chprintf.h>

#include <motors.h>
#include <mode.h>
#include <audio/microphone.h>
#include <audio_processing.h>
#include <fft.h>
#include <arm_math.h>
#include <figure.h>
#include <mode.h>


//semaphore
//static BSEMAPHORE_DECL(sendToComputer_sem, TRUE);

//2 times FFT_SIZE because these arrays contain complex numbers (real + imaginary)
static float micLeft_cmplx_input[2 * FFT_SIZE];
static float micRight_cmplx_input[2 * FFT_SIZE];
static float micFront_cmplx_input[2 * FFT_SIZE];
static float micBack_cmplx_input[2 * FFT_SIZE];
//Arrays containing the computed magnitude of the complex numbers
static float micLeft_output[FFT_SIZE];
static float micRight_output[FFT_SIZE];
static float micFront_output[FFT_SIZE];
static float micBack_output[FFT_SIZE];


#define NORM_TABLE_SIZE	((int)(17))
#define INITIAL_TABLE_COUNTER_VALUE ((uint8_t)(0))
#define	DEFAULT_MEAN	((uint8_t)(0))

#define SOUND_DETECTED			1
#define SOUND_NOT_DETECTED		0
#define CHECK_SENSIBILITY		NORM_TABLE_SIZE - MAX_UNCORRECT_SAMPLES
#define	MAX_UNCORRECT_SAMPLES	5
#define ERROR_FREQ				1
#define INITIAL_COUNTER_VALUE	0
#define	NORM_RESET_VALUE		0

#define MIN_VALUE_THRESHOLD	10000 

#define MIN_FREQ		10	//we don't analyze before this index to not use resources for nothing
#define FREQ_SQUARE		16	//250Hz
#define FREQ_TRIANGLE	19	//296Hz
#define FREQ_CIRCLE		23	//359HZ
#define FREQ_SIZE_1		26	//406Hz
#define	FREQ_SIZE_2		29	//453Hz
#define FREQ_SIZE_MAX	32	//500Hz
#define MAX_FREQ		35	//we don't analyze after this index to not use resources for nothing


static uint16_t max_norm_table[NORM_TABLE_SIZE] = {0};


//uint8_t max_norm_mean(void);
void max_norm_buff_update(uint8_t max_norm_new_index);
uint8_t max_norm_check(uint8_t frequence);


/*
*	Simple function used to detect the highest value in a buffer
*	and to execute a motor command depending on it
*/
void sound_remote(float* data){
	float max_norm = MIN_VALUE_THRESHOLD;
	int16_t max_norm_index = -1; 
	//uint8_t mean;

	//search for the highest peak
	for(uint16_t i = MIN_FREQ ; i <= MAX_FREQ ; i++){
		if(data[i] > max_norm){
			max_norm = data[i];
			max_norm_index = i;
		}
	}

	max_norm_buff_update(max_norm_index);
	//mean = max_norm_mean();

/*	//draw a square
	if(mean >= FREQ_FORWARD_L && mean <= FREQ_FORWARD_H){
		figure_size_set(FIGURE_SIZE_1);
		figure_set(FIGURE_SQUARE);
	}
	//draw a triangle
	else if(mean >= FREQ_LEFT_L && mean <= FREQ_LEFT_H){
		figure_size_set(FIGURE_SIZE_1);
		figure_set(FIGURE_TRIANGLE);
	}
	//draw a circle
	else if(mean >= FREQ_RIGHT_L && mean <= FREQ_RIGHT_H){
		figure_size_set(FIGURE_SIZE_1);
		figure_set(FIGURE_CIRCLE);
	}
*/

	//draw a square
	if(max_norm_check(FREQ_SQUARE)){
		figure_set(FIGURE_SQUARE);
	}
	//draw a triangle
	else if(max_norm_check(FREQ_TRIANGLE)){
		figure_set(FIGURE_TRIANGLE);
	}
	//draw a circle
	else if(max_norm_check(FREQ_CIRCLE)){
		figure_set(FIGURE_CIRCLE);
	}
	//draw with size 1
	else if(max_norm_check(FREQ_SIZE_1)){
			figure_size_set(FIGURE_SIZE_1);
	}
	//draw with size 2
	else if(max_norm_check(FREQ_SIZE_2)){
			figure_size_set(FIGURE_SIZE_2);
	}
	//draw with size 3
	else if(max_norm_check(FREQ_SIZE_MAX)){
			figure_size_set(FIGURE_SIZE_MAX);
	}
}

/*
*	Callback called when the demodulation of the four microphones is done.
*	We get 160 samples per mic every 10ms (16kHz)
*	
*	params :
*	int16_t *data			Buffer containing 4 times 160 samples. the samples are sorted by micro
*							so we have [micRight1, micLeft1, micBack1, micFront1, micRight2, etc...]
*	uint16_t num_samples	Tells how many data we get in total (should always be 640)
*/
void processAudioData(int16_t *data, uint16_t num_samples){

	/*
	*
	*	We get 160 samples per mic every 10ms
	*	So we fill the samples buffers to reach
	*	1024 samples, then we compute the FFTs.
	*
	*/

	static uint16_t nb_samples = 0;
	static uint8_t mustSend = 0;
	if(mode_get()== MODE_IDLE) //sound processing needs to be done only when in MODE_IDLE
	{
		//loop to fill the buffers
		for(uint16_t i = 0 ; i < num_samples ; i+=4){
			//construct an array of complex numbers. Put 0 to the imaginary part
			micRight_cmplx_input[nb_samples] = (float)data[i + MIC_RIGHT];
			micLeft_cmplx_input[nb_samples] = (float)data[i + MIC_LEFT];
			micBack_cmplx_input[nb_samples] = (float)data[i + MIC_BACK];
			micFront_cmplx_input[nb_samples] = (float)data[i + MIC_FRONT];

			nb_samples++;

			micRight_cmplx_input[nb_samples] = 0;
			micLeft_cmplx_input[nb_samples] = 0;
			micBack_cmplx_input[nb_samples] = 0;
			micFront_cmplx_input[nb_samples] = 0;

			nb_samples++;

			//stop when buffer is full
			if(nb_samples >= (2 * FFT_SIZE)){
				break;
			}
		}

		if(nb_samples >= (2 * FFT_SIZE)){
			/*	FFT proccessing
			*
			*	This FFT function stores the results in the input buffer given.
			*	This is an "In Place" function.
			*/

			doFFT_optimized(FFT_SIZE, micRight_cmplx_input);
			doFFT_optimized(FFT_SIZE, micLeft_cmplx_input);
			doFFT_optimized(FFT_SIZE, micFront_cmplx_input);
			doFFT_optimized(FFT_SIZE, micBack_cmplx_input);

			/*	Magnitude processing
			*
			*	Computes the magnitude of the complex numbers and
			*	stores them in a buffer of FFT_SIZE because it only contains
			*	real numbers.
			*
			*/
			arm_cmplx_mag_f32(micRight_cmplx_input, micRight_output, FFT_SIZE);
			arm_cmplx_mag_f32(micLeft_cmplx_input, micLeft_output, FFT_SIZE);
			arm_cmplx_mag_f32(micFront_cmplx_input, micFront_output, FFT_SIZE);
			arm_cmplx_mag_f32(micBack_cmplx_input, micBack_output, FFT_SIZE);

			//sends only one FFT result over 10 for 1 mic to not flood the computer
			//sends to UART3
			/*if(mustSend > 8){
				//signals to send the result to the computer
				chBSemSignal(&sendToComputer_sem);
				mustSend = 0;
			} */
			nb_samples = 0;
			mustSend++;

			sound_remote(micLeft_output);
		}
	}
}

/*void wait_send_to_computer(void){
	chBSemWait(&sendToComputer_sem);
} */

float* get_audio_buffer_ptr(BUFFER_NAME_t name){
	if(name == LEFT_CMPLX_INPUT){
		return micLeft_cmplx_input;
	}
	else if (name == RIGHT_CMPLX_INPUT){
		return micRight_cmplx_input;
	}
	else if (name == FRONT_CMPLX_INPUT){
		return micFront_cmplx_input;
	}
	else if (name == BACK_CMPLX_INPUT){
		return micBack_cmplx_input;
	}
	else if (name == LEFT_OUTPUT){
		return micLeft_output;
	}
	else if (name == RIGHT_OUTPUT){
		return micRight_output;
	}
	else if (name == FRONT_OUTPUT){
		return micFront_output;
	}
	else if (name == BACK_OUTPUT){
		return micBack_output;
	}
	else{
		return NULL;
	}
}

void max_norm_buff_update(uint8_t max_norm_new_index){
	static uint8_t table_counter = INITIAL_TABLE_COUNTER_VALUE;

	max_norm_table[table_counter] = max_norm_new_index;
	if(++table_counter == NORM_TABLE_SIZE) table_counter = INITIAL_TABLE_COUNTER_VALUE;
}

/*uint8_t max_norm_mean(void){
	uint8_t mean = DEFAULT_MEAN;
	for(int i = 0; i < NORM_TABLE_SIZE; i++){
		mean += max_norm_table[i];
	}
	mean /= NORM_TABLE_SIZE;
	return mean;
}*/

uint8_t max_norm_check(uint8_t frequence){
	uint8_t counter = INITIAL_COUNTER_VALUE;
	for(int i = 0; i < NORM_TABLE_SIZE; i++){
		if(max_norm_table[i] >= (frequence - ERROR_FREQ) && max_norm_table[i] <= (frequence + ERROR_FREQ) ) counter++;
	}
	if(counter >= CHECK_SENSIBILITY) return SOUND_DETECTED;
	else return SOUND_NOT_DETECTED;
}

void max_norm_buff_reset(void){
	for(int i = 0;i<NORM_TABLE_SIZE;i++){
		max_norm_table[i] = NORM_RESET_VALUE;
	}
}

