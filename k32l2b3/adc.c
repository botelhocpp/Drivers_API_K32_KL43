#include "adc.h"

#include <stdbool.h>

long adcReadInput(adc_channel channel) {
	// START CONVERSION ON THE GIVEN CHANNEL
	ADC0->SC1[0] = channel;

	// WAIT FOR CONVERSION TO COMPLETE
	while(!(ADC0->SC1[0] & (1U << 7)));

	// READ CONVERSION RESULT AND CLEAR COCO FLAG
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
	// ENABLE CLOCK TO ADC0
	SIM->SCGC6 |= (1U << 27);

	// SOFTWARE TRIGGER
	ADC0->SC2 = (adc->hardware_trigger) ?
			ADC->SC2 | (1U << 6) :
			ADC->SC2 & ~(1U << 6);

	// AVERAGE OF 32 SAMPLES
	ADC0->SC3 |= (0b111 << 0);

	// CLOCK DIVIDED BY 4
	ADC0->CFG1 |= (0b10U << 5);

	// LONG SAMPLE TIME
	ADC0->CFG1 |= (1U << 4);

	// SINGLE ENDED 16-BITS
	ADC0->CFG1 |= (0b11U << 2);
}
