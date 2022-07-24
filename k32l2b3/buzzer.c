#include "k32l2b3/buzzer.h"

#include "k32l2b3/tpm.h"
#include "k32l2b3/clock.h"
#include "k32l2b3/delay.h"

#include <stdint.h>

void buzzerGetDefaultConfig(buzzer_handler_t *buzzer) {
	buzzer->module = tpmMODULE_0;
	buzzer->channel = tpmCHANNEL_0;
	buzzer->clk = tpmCLOCK_MCGIRCLK;
	buzzer->ps = tpmPRESCALER_128;
	buzzer->channel_mode = tpmCHANNEL_EDGE_ALIGNED_PWM_HT;
}

void buzzerInitModule(buzzer_handler_t *buzzer) {
	uint32_t source_clock = tpmGetSourceClock(buzzer);
	tpmInitModule(buzzer);
	tpmSetModuleValue(buzzer, source_clock/NOTE_C3);
	tpmSetChannelValue(buzzer, tpmGetModuleValue(buzzer)/2);
}

void buzzerTone(buzzer_handler_t *buzzer, uint32_t note, uint32_t duration) {
	if(note != NO_NOTE) {
		uint32_t source_clock = tpmGetSourceClock(buzzer);
		tpmStartTimer(buzzer);
		tpmSetModuleValue(buzzer, source_clock/note);
		tpmSetChannelValue(buzzer, tpmGetModuleValue(buzzer)/2);
		delay_ms(duration);
		tpmStopTimer(buzzer);
	}
	else {
		delay_ms(duration);
	}
}
