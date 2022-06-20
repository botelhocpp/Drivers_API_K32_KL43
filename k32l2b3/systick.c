#include "systick.h"

#include "clock.h"

#include <stdbool.h>

void systickInitTimer(systick_clock clock, long ms) {

	// CONFIG THE GIVEN CLOCK IN THE TIMER AND GETS THE FREQUENCY
	long systick_clock = 0;
	switch(clock) {
		case systickERCLK:
			systick_clock = clkGetERClock();
			SysTick->CTRL &= ~(1U << 2);
			break;
		case systickMainCLK:
			systick_clock = clkGetMainClock();
			SysTick->CTRL |= (1U << 2);
			break;
		default:
			while(1);
	}

	// SETS THE LOAD REGISTER TO COUNT THE GIVEN TIME
	SysTick->LOAD = ms*(systick_clock/1000);

	// EMPTY THE VAL REGISTER
	SysTick->VAL = 0;

	// ENABLES THE SYSTICK EXCEPTION WHEN COUNT REACHES 0
	SysTick->CTRL |= (1U << 1);
}

bool systickCheckFlag() {
	// CHECK THE COUNT FLAG
	return SysTick->CTRL & (1U << 16);
}

void systickStartTimer()  {
	// ENABLE THE COUNTER
	SysTick->CTRL |= (1U << 0);
}

void systickStopTimer() {
	// DISABLE THE COUNTER
	SysTick->CTRL &= ~(1U << 0);
}
