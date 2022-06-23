#ifndef DELAY_H
#define DELAY_H

#include <stdint.h>

/**!
 * @brief 	Makes the CPU wait for a given time in
 * 		  	milliseconds (occupies the CPU during
 * 		  	that time).
 * @parameters The delay time in milliseconds.
 */
extern void delay_ms(uint32_t ms);

/**!
 * @brief 	Makes the CPU wait for a given time in
 * 		  	microseconds (occupies the CPU during
 * 		  	that time).
 * @parameters The delay time in microseconds.
 */
extern void delay_us(uint32_t us);

#endif
