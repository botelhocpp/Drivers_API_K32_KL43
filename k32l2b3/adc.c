#include "K32L2B31A.h"

#include "k32l2b3/adc.h"
#include "k32l2b3/clock.h"

#include <stdbool.h>

long adcReadInput(adc_channel channel) {
	ADC0->SC1[0] = channel;
	while(!(ADC0->SC1[0] & (1U << 7)));
	return ADC0->R[0];
}

void adcGetDefaultConfig(adc_config_t *adc) {
	adc->clock_div = adcCLOCK_DIVIDE_BY_4;
	adc->long_sample_time = true;
	adc->resolution = adcRESOLUTION_16_BIT;
	adc->sample_number = adcSAMPLE_NUMBER_32;
	adc->hardware_trigger = false;
}

void adcInitModule(adc_config_t *adc) {
	clkEnablePeripheralClock(clkADC_0);

	ADC0->SC3 |= (adc->sample_number << 0);
	ADC0->CFG1 |= (adc->clock_div << 5) | (adc->resolution << 2);

	ADC0->SC2 = (adc->hardware_trigger) ?
			ADC0->SC2 | (1U << 6) :
			ADC0->SC2 & ~(1U << 6);

	ADC0->SC2 = (adc->long_sample_time) ?
			ADC0->SC2 | (1U << 4) :
			ADC0->SC2 & ~(1U << 4);
}
