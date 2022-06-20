#ifndef SYSTICK_H
#define SYSTICK_H

#include <stdbool.h>

typedef enum {
	systickERCLK,
	systickMainCLK
} systick_clock;

extern void systickInitTimer(systick_clock clock, long ms);

extern bool systickCheckFlag();

extern void systickStartTimer();

extern void systickStopTimer();

#endif
