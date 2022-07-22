#ifndef ADC_H
#define ADC_H

#include <stdbool.h>

typedef enum {
	adcSAMPLE_NUMBER_0 = -1,
	adcSAMPLE_NUMBER_4,
	adcSAMPLE_NUMBER_8,
	adcSAMPLE_NUMBER_16,
	adcSAMPLE_NUMBER_32
} adc_sample_number;

typedef enum {
	adcRESOLUTION_8_BIT,
	adcRESOLUTION_12_BIT,
	adcRESOLUTION_10_BIT,
	adcRESOLUTION_16_BIT
} adc_resolution;

typedef enum {
	adcCLOCK_DIVIDE_BY_1,
	adcCLOCK_DIVIDE_BY_2,
	adcCLOCK_DIVIDE_BY_4,
	adcCLOCK_DIVIDE_BY_8
} adc_clock_div;

typedef enum {
	adcCHANNEL_0,
	adcCHANNEL_1,
	adcCHANNEL_2,
	adcCHANNEL_3,
	adcCHANNEL_4,
	adcCHANNEL_5,
	adcCHANNEL_6,
	adcCHANNEL_7,
	adcCHANNEL_8,
	adcCHANNEL_9
} adc_channel;

typedef enum {
	adcCLOCK_BUS,
	adcCLOCK_BUS_DIV_2,
	adcCLOCK_ALT,
	adcCLOCK_ASYNC
} adc_clock;

typedef struct {
	adc_clock_div clock_div;
	adc_sample_number sample_number;
	bool long_sample_time;
	adc_resolution resolution;
	bool hardware_trigger;
	adc_clock input_clock;
} adc_config_t;

void adcGetDefaultConfig(adc_config_t *adc);

void adcInitModule(adc_config_t *adc);

long adcReadInput(adc_channel channel);

bool adcCalibrate(void);

#endif
