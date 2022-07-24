#include "k32l2b3/common.h"
#include "k32l2b3/servo.h"
#include "k32l2b3/tpm.h"

#define SERVO_PWM_FREQUENCY 		50
#define SERVO_PWM_PERIOD 			20

#define SERVO_PWM_MINIMUM_ANGLE 	0.5
#define SERVO_PWM_MAXIMUM_ANGLE 	2.5

#define MAXIMUM_SERVO_NUMBER 		TPM_CHANNEL_NUMBER*TPM_MODULE_NUMBER

static uint16_t minimum_values[MAXIMUM_SERVO_NUMBER];
static uint16_t maximum_values[MAXIMUM_SERVO_NUMBER];

void servoGetDefaultConfig(servo_handler_t *servo) {
	servo->module = tpmMODULE_0;
	servo->channel = tpmCHANNEL_0;
	servo->clk = tpmCLOCK_MCGIRCLK;
	servo->ps = tpmPRESCALER_64;
#if PWM_3V3
	servo->channel_mode = tpmCHANNEL_EDGE_ALIGNED_PWM_HT;
#else
	servo->channel_mode = tpmCHANNEL_EDGE_ALIGNED_PWM_LT;
#endif
}

void servoInitModule(servo_handler_t *servo) {
	tpmInitModule(servo);

	uint32_t source_clock = tpmGetSourceClock(servo);
	uint16_t tpm_mod = (uint16_t) (source_clock/SERVO_PWM_FREQUENCY) - 1;
	uint8_t tpm_index = TPM_CHANNEL_NUMBER*servo->module + servo->channel;

	minimum_values[tpm_index] = (uint16_t) (tpm_mod*SERVO_PWM_MINIMUM_ANGLE)/SERVO_PWM_PERIOD;
	maximum_values[tpm_index] = (uint16_t) (tpm_mod*SERVO_PWM_MAXIMUM_ANGLE)/SERVO_PWM_PERIOD;

	tpmSetModuleValue(servo, tpm_mod);
	tpmSetChannelValue(servo, minimum_values[tpm_index]);

	tpmStartTimer(servo);
}

void servoWrite(servo_handler_t *servo, uint16_t value) {
	uint8_t tpm_index = TPM_CHANNEL_NUMBER*servo->module + servo->channel;
	uint16_t minimum_value = minimum_values[tpm_index];
	uint16_t maximum_value = maximum_values[tpm_index];

	if(value <= maximum_value && value >= minimum_value) {
		tpmSetChannelValue(servo, value);
	}
	else if(value > maximum_value) {
		tpmSetChannelValue(servo, maximum_value);
	}
	else {
		tpmSetChannelValue(servo, minimum_value);
	}
}

void servoWriteAngle(servo_handler_t *servo, uint8_t angle) {
	uint8_t tpm_index = TPM_CHANNEL_NUMBER*servo->module + servo->channel;
	uint16_t servo_value = map(angle, 0, 100, minimum_values[tpm_index], maximum_values[tpm_index]);
	tpmSetChannelValue(servo, servo_value);
}

uint16_t servoGetCurrentValue(servo_handler_t *servo) {
	return tpmGetChannelValue(servo);
}

uint8_t servoGetCurrentAngle(servo_handler_t *servo) {
	uint8_t tpm_index = TPM_CHANNEL_NUMBER*servo->module + servo->channel;
	uint16_t servo_value = tpmGetChannelValue(servo);
	return map(servo_value, minimum_values[tpm_index], maximum_values[tpm_index], 0, 100);
}

void servoSetMinimumValue(servo_handler_t *servo, uint16_t minimum_value) {
	uint8_t tpm_index = TPM_CHANNEL_NUMBER*servo->module + servo->channel;
	uint16_t absolute_minimum_value = (uint16_t) (tpmGetModuleValue(servo)*SERVO_PWM_MINIMUM_ANGLE)/SERVO_PWM_PERIOD;
	if(minimum_value > absolute_minimum_value && minimum_value < maximum_values[tpm_index]) {
		minimum_values[tpm_index] = minimum_value;
	}
}

void servoSetMaximumValue(servo_handler_t *servo, uint16_t maximum_value) {
	uint8_t tpm_index = TPM_CHANNEL_NUMBER*servo->module + servo->channel;
	uint16_t absolute_maximum_value = (uint16_t) (tpmGetModuleValue(servo)*SERVO_PWM_MAXIMUM_ANGLE)/SERVO_PWM_PERIOD;
	if(maximum_value < absolute_maximum_value && maximum_value > minimum_values[tpm_index]) {
		maximum_values[tpm_index] = maximum_value;
	}
}

uint16_t servoGetMinimumValue(servo_handler_t *servo) {
	uint8_t tpm_index = TPM_CHANNEL_NUMBER*servo->module + servo->channel;
	return minimum_values[tpm_index];
}

uint16_t servoGetMaximumValue(servo_handler_t *servo) {
	uint8_t tpm_index = TPM_CHANNEL_NUMBER*servo->module + servo->channel;
	return maximum_values[tpm_index];
}

void servoSetMinimumAngle(servo_handler_t *servo, uint8_t minimum_angle) {
	uint8_t tpm_index = TPM_CHANNEL_NUMBER*servo->module + servo->channel;
	uint16_t new_minimum_value = map(minimum_angle, 0, 100, minimum_values[tpm_index], maximum_values[tpm_index]);
	if(new_minimum_value < maximum_values[tpm_index]) {
		minimum_values[tpm_index] = new_minimum_value;
	}
}

void servoSetMaximumAngle(servo_handler_t *servo, uint8_t maximum_angle) {
	uint8_t tpm_index = TPM_CHANNEL_NUMBER*servo->module + servo->channel;
	uint16_t new_maximum_value = map(maximum_angle, 0, 100, minimum_values[tpm_index], maximum_values[tpm_index]);
	if(new_maximum_value > minimum_values[tpm_index]) {
		maximum_values[tpm_index] = new_maximum_value;
	}
}

uint8_t servoGetMinimumAngle(servo_handler_t *servo) {
	uint8_t tpm_index = TPM_CHANNEL_NUMBER*servo->module + servo->channel;

	uint16_t absolute_minimum_value = (uint16_t) (tpmGetModuleValue(servo)*SERVO_PWM_MINIMUM_ANGLE)/SERVO_PWM_PERIOD;
	uint16_t absolute_maximum_value = (uint16_t) (tpmGetModuleValue(servo)*SERVO_PWM_MAXIMUM_ANGLE)/SERVO_PWM_PERIOD;

	return map(minimum_values[tpm_index], absolute_minimum_value, absolute_maximum_value, 0, 100);
}

uint8_t servoGetMaximumAngle(servo_handler_t *servo) {
	uint8_t tpm_index = TPM_CHANNEL_NUMBER*servo->module + servo->channel;

	uint16_t absolute_minimum_value = (uint16_t) (tpmGetModuleValue(servo)*SERVO_PWM_MINIMUM_ANGLE)/SERVO_PWM_PERIOD;
	uint16_t absolute_maximum_value = (uint16_t) (tpmGetModuleValue(servo)*SERVO_PWM_MAXIMUM_ANGLE)/SERVO_PWM_PERIOD;

	return map(maximum_values[tpm_index], absolute_minimum_value, absolute_maximum_value, 0, 100);
}

