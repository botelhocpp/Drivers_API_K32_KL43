/**
 * @file    port.h
 * @brief   Carries out all the control of the
 *          PORT peripheral.
 * @author  Pedro Botelho
 */

#ifndef PORT_H
#define PORT_H

#include "K32L2B31A.h"

#include <stdint.h>
#include <stdbool.h>

typedef enum {
	portINT_LOGIC_0 = 0b1000,
	portINT_RISING_EDGE = 0b1001,
	portINT_FALLING_EDGE = 0b1010,
	portINT_EITHER_EDGE = 0b1011,
	portINT_LOGIC_1 = 0b1100
} port_interrupt;

typedef enum {
	pinPORT_A,
	pinPORT_B,
	pinPORT_C,
	pinPORT_D,
	pinPORT_E
} pin_port;

typedef struct {
	pin_port port;
	uint8_t pin;
} pin_handler_t;

// ================================================================================
// VALIDATION FUNCTIONS
// ================================================================================

extern bool portPinNumberValidate(uint8_t pin_number);

extern bool portPortValidate(pin_port port);

extern bool portPinValidate(pin_handler_t *io_pin);

// ================================================================================
// PORT CONFIGURATION FUNCTIONS
// ================================================================================

extern void portInitPort(pin_port port);

extern void portSetMuxGpio(pin_handler_t *io_pin);

extern void portConfigInterrupt(pin_handler_t *inputPin, port_interrupt interrupt);

extern void portConfigPullup(pin_handler_t *inputPin);

// ================================================================================
// INTERRUPT STATUS FUNCTION
// ================================================================================

extern bool portCheckInterrupt(pin_handler_t *io_pin);

#endif
