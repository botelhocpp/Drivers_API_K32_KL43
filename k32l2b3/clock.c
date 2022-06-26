#include <k32l2b3/clock.h>

#include "K32L2B31A.h"

#define SET_MASK(value, mask, starting_bit) ({\
												value &= ~(~mask << starting_bit);\
												value |=  ( mask << starting_bit);\
											  })

// ================================================================================
// MCG_Lite REGISTERS MASKS
// ================================================================================
#define C1_IRCLKEN  		(1U << 1)
#define C1_IREFSTEN 		(1U << 0)

#define C2_EREFS0     		(1U << 2)
#define C2_IRCS     		(1U << 0)

#define S_OSCINIT0     		(1U << 1)

#define MC_HIRCEN     		(1U << 7)

// ================================================================================
// OSC REGISTERS MASKS
// ================================================================================
#define OSC0_ERCLKEN    	(1U << 7)
#define OSC0_EREFSTEN    	(1U << 5)

// ================================================================================
// CLOCK FUNCTIONS
// ================================================================================

void clkEnablePeripheralClock(clock_peripheral peripheral) {
	if(peripheral < 0 || peripheral > 63) {
		while(1);
	}
	if(peripheral < 32) {
		SIM->SCGC5 |= (1U << peripheral);
	}
	else {
		SIM->SCGC6 |= (1U << (peripheral - 32));
	}

}

void clkEnableClock(clock_source clock_source) {
	switch(clock_source) {
		case clkSOURCE_LIRCLK_2MHz:
			MCG->C1 |=  C1_IRCLKEN | C1_IREFSTEN;		// ENABLES LIRC CLOCK SOURCE AND SET TO STAY ENABLED EVEN IN STOP MODE.
			MCG->C2 &= ~C2_IRCS; 						// SELECTS LIRC TO OPERATE IN 2MHz.
			break;
		case clkSOURCE_LIRCLK_8MHz:
			MCG->C1 |=  C1_IRCLKEN | C1_IREFSTEN; 		// ENABLES LIRC CLOCK SOURCE AND SET TO STAY ENABLED EVEN IN STOP MODE.
			MCG->C2 |=  C2_IRCS; 						// SELECTS LIRC TO OPERATE IN 8MHz.
			break;
		case clkSOURCE_HIRCLK:
			MCG->MC |=  MC_HIRCEN; 						// ENABLES HIRC CLOCK SOURCE.
			break;
		case clkSOURCE_OSCERCLK:
			OSC0->CR |= OSC0_ERCLKEN | OSC0_EREFSTEN; 	// ENABLES ERCLK AND SET OSCERCLK TO STAY ENABLED EVEN IN STOP MODE.
			MCG->C2  |= C2_EREFS0; 						// SET THE EXT CLOCK SOURCE TO THE OSCILLATOR.
			while(!(MCG->S & S_OSCINIT0)); 				// WAIT FOR OSCERCLK TO BE READY.
			break;
		default:
			while(1);
	}
}


void clkSetMainClock(clock_source clock_source) {
	switch(clock_source) {
		case clkSOURCE_LIRCLK_2MHz:
		case clkSOURCE_LIRCLK_8MHz:
			SET_MASK(MCG->C1, 0b01, 6);									// SELECT LIRC AS THE MAIN CLOCK SOURCE.
			while(!(MCG->S & (0b01 << 2))); 							// WAIT CLOCK MODE BE SET TO LIRC.
			break;
		case clkSOURCE_HIRCLK:
			SET_MASK(MCG->C1, 0b00, 6);									// SELECT HIRC AS THE MAIN CLOCK SOURCE.
			while((MCG->S & (0b10 << 2)) || (MCG->S & (0b01 << 2)));	// WAIT CLOCK MODE BE SET TO HIRC.
			break;
		case clkSOURCE_OSCERCLK:
			SET_MASK(MCG->C1, 0b10, 6);									// SELECT OSCERCLK AS THE MAIN CLOCK SOURCE.
			while(!(MCG->S & (0b10U << 2))); 							// WAIT CLOCK MODE BE SET TO OSCERCLK.
			break;
		default:
			while(1);
	}
}

long clkGetMainClock() {
	int clock_status = (MCG->S & 0b11U << 2) >> 2;
	switch(clock_status) {
		case 0:
			return (MCG->MC & (1U << 7)) ? CLK_SOURCE_HIRCLK_FREQ : 0;
		case 1:

			// LIRC DISABLED
			if(!(MCG->C1 & 1U << 1)) {
				return 0;
			}
			return (MCG->C2 & 1) ? CLK_SOURCE_LIRCLK_8MHz_FREQ : CLK_SOURCE_LIRCLK_2MHz_FREQ;
		case 2:

			// OSCILLATOR
			if(MCG->C2 & 1U << 2) {
				return ((MCG->S & (1U << 1)) && (OSC0->CR & (1U << 7))) ? CLK_SOURCE_OSCERCLK_FREQ : 0;
			}
			// EXTERNAL CLOCK
			else {
				// TODO
			}
	}
	return 0;
}

long clkGetERClock() {
	if(MCG->C2 & 1U << 2) {
		return (OSC0->CR & (1U << 7)) ? CLK_SOURCE_OSCERCLK_FREQ : 0;
	}
	// EXTERNAL CLOCK
	else {
		// TODO
	}
	return 0;
}
