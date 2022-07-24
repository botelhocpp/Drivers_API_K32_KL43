#ifndef SERVO_H
#define SERVO_H

#include "k32l2b3/tpm.h"

/**
 * Defines if it's been used a buffer to attain a 5V
 * PWM, for 0, or is using the direct 3V3 output from
 * the pin, for 1.
 */
#define PWM_3V3	0

typedef tpm_handler_t servo_handler_t;

extern void servoGetDefaultConfig(servo_handler_t *servo);

extern void servoInitModule(servo_handler_t *servo);

extern void servoWrite(servo_handler_t *servo, uint16_t value);

extern uint16_t servoGetCurrentValue(servo_handler_t *servo);

extern uint8_t servoGetCurrentAngle(servo_handler_t *servo);

extern void servoWriteAngle(servo_handler_t *servo, uint8_t angle);

extern void servoSetMinimumValue(servo_handler_t *servo, uint16_t minimum_value);

extern void servoSetMaximumValue(servo_handler_t *servo, uint16_t maximum_value);

extern uint16_t servoGetMinimumValue(servo_handler_t *servo);

extern uint16_t servoGetMaximumValue(servo_handler_t *servo);

extern void servoSetMinimumAngle(servo_handler_t *servo, uint8_t minimum_angle);

extern void servoSetMaximumAngle(servo_handler_t *servo, uint8_t maximum_angle);

extern uint8_t servoGetMinimumAngle(servo_handler_t *servo);

extern uint8_t servoGetMaximumAngle(servo_handler_t *servo);

#endif
