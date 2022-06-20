#include "delay.h"
#include "k32_api/clock.h"

/*
 * Tempo = Nciclocs x (1/f)
 * tempo/(f) = Nciclos
 * tempo_ms*f/1000 = Nciclos
 * 1ms*(8.000.000)/3*1000 = 2666,67
 */
void delay_ms(uint32_t ms) {
	int iterations = (int) clkGetMainClock()/3000;

	for(int i = 0; i < ms; i++) {
		for(int j = 0; j < iterations; j++);
	}
}

void delay_us(uint32_t us) {
	int iterations = (int) clkGetMainClock()/3000000;

	for(int i = 0; i < us; i++) {
		for(int j = 0; j < iterations; j++);
	}
}
