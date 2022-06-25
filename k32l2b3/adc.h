#ifndef ADC_H
#define ADC_H

#include <stdbool.h>

typedef enum {
	adcSAMPLE_NUMBER_32
} adc_sample_number;

typedef enum {
	adcRESOLUTION_8_BIT,
	adcRESOLUTION_10_BIT,
	adcRESOLUTION_12_BIT,
	adcRESOLUTION_16_BIT
} adc_resolution;

typedef enum {
	adcCLOCK_DIVIDE_BY_4
} adc_clock_div;

typedef enum {
	adcCHANNEL_0,
	adcCHANNEL_1,
	adcCHANNEL_2,
	adcCHANNEL_3
} adc_channel;

typedef struct {
	adc_channel channel;
	adc_clock_div clock_div;
	adc_sample_number sample_number;
	adc_clock_div clock_div;
	bool long_sample_time;
	adc_resolution resolution;
	bool hardware_trigger;
} adc_config_t;

void adcGetDefaultConfig(adc_config_t *adc);

void adcInitModule(adc_config_t *adc);

long adcReadInput(adc_channel channel);

#endif
