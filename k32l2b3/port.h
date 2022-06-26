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
	pinMUX_ALT_0 = 0b000,
	pinMUX_ALT_1 = 0b001,
	pinMUX_ALT_2 = 0b010,
	pinMUX_ALT_3 = 0b011,
	pinMUX_ALT_4 = 0b100,
	pinMUX_ALT_5 = 0b101,
	pinMUX_ALT_6 = 0b110,
	pinMUX_ALT_7 = 0b111
} pin_mux;

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

extern void portSetPinMux(pin_port port, uint8_t pin_number, pin_mux pin_function);

extern void portSetMux(pin_handler_t *io_pin, pin_mux pin_function);

extern void portSetMuxGpio(pin_handler_t *io_pin);

extern void portConfigInterrupt(pin_handler_t *inputPin, port_interrupt interrupt);

extern void portConfigPullup(pin_handler_t *inputPin);

// ================================================================================
// INTERRUPT STATUS FUNCTION
// ================================================================================

extern bool portCheckInterrupt(pin_handler_t *io_pin);

extern void portClearInterrupt(pin_handler_t *io_pin);

#endif
