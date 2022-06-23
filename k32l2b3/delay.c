#include "delay.h"
#include "clock.h"

/**!
 * @brief 	Makes the CPU wait for a given time in
 * 		  	milliseconds (occupies the CPU during
 * 		  	that time).
 * @parameters The delay time in milliseconds.
 */
void delay_ms(uint32_t ms) {
	int iterations = (int) clkGetMainClock()/3000;

	for(int i = 0; i < ms; i++) {
		// tempo_ms*f/1000 = N_cycles = 3*iterations
		for(int j = 0; j < iterations; j++);
	}
}

/**!
 * @brief 	Makes the CPU wait for a given time in
 * 		  	microseconds (occupies the CPU during
 * 		  	that time).
 * @parameters The delay time in microseconds.
 */
void delay_us(uint32_t us) {
	int iterations = (int) clkGetMainClock()/3000000;

	for(int i = 0; i < us; i++) {
		// tempo_us*f/1000000 = N_cycles = 3*iterations
		for(int j = 0; j < iterations; j++);
	}
}
