/**
 * @file    port.c
 * @brief   Carries out all the control of the
 *          PORT peripheral.
 * @author  Pedro Botelho
 */

#include <k32l2b3/clock.h>
#include <k32l2b3/port.h>
#include "K32L2B31A.h"

#include <stdint.h>
#include <stdbool.h>


static PORT_Type *pin_ports[5] = {PORTA, PORTB, PORTC, PORTD, PORTE};

// ================================================================================
// PRIVATE FUNCTIONS PROTOTYPES
// ================================================================================

PORT_Type *portGetPinPort(pin_handler_t *io_pin);

// ================================================================================
// VALIDATION FUNCTIONS
// ================================================================================

bool portPinNumberValidate(uint8_t pin_number) {
	return (pin_number < 32 || pin_number >= 0);
}

bool portPortValidate(pin_port port) {
	return (port < 5 || port >= 0);
}

bool portPinValidate(pin_handler_t *io_pin) {
	return portPortValidate(io_pin->port) && portPinNumberValidate(io_pin->pin);
}

// ================================================================================
// PORT CONFIGURATION FUNCTIONS
// ================================================================================

void portInitPort(pin_port port) {
	if(!portPortValidate(port)) {
		while(1);
	}
	clkEnablePeripheralClock(clkPORT_A + port);
}

void portSetPinMux(pin_port port, uint8_t pin_number, pin_mux pin_function) {
	if(!portPinNumberValidate(pin_number) || !portPortValidate(port)) {
		while(1);
	}
	portInitPort(port);
	PORT_Type *pinPort = pin_ports[port];
	pinPort->PCR[pin_number] &= ~(~pin_function << 8);
	pinPort->PCR[pin_number] |=  (pin_function << 8);
}

void portSetMux(pin_handler_t *io_pin, pin_mux pin_function) {
	PORT_Type *pinPort = portGetPinPort(io_pin);
	// 0b0000_0101_0000_0000
	pinPort->PCR[io_pin->pin] &= ~(~pin_function << 8);
	pinPort->PCR[io_pin->pin] |=  ( pin_function << 8);
}

void portSetMuxGpio(pin_handler_t *io_pin) {
	portSetMux(io_pin, pinMUX_ALT_1);
}

void portConfigInterrupt(pin_handler_t *inputPin, port_interrupt interrupt) {
	if(interrupt < portINT_LOGIC_0 || interrupt > portINT_LOGIC_1) {
		while(1);
	}
	PORT_Type *pinPort = portGetPinPort(inputPin);
	pinPort->PCR[inputPin->pin] |= (interrupt << 16);
	pinPort->PCR[inputPin->pin] &= ~(~interrupt << 16);

	uint32_t int_mask = 0;
	if(inputPin->port == pinPORT_A) {
		int_mask = (1U << 30);
	}
	else if(inputPin->port == pinPORT_C || inputPin->port == pinPORT_D) {
		int_mask = (1U << 31);
	}
	else {
		while(1);
	}
	NVIC->ISER[0] |= int_mask;
}


void portConfigPullup(pin_handler_t *inputPin) {
	PORT_Type *pinPort = portGetPinPort(inputPin);
	pinPort->PCR[inputPin->pin] |= (1U << 1);
	pinPort->PCR[inputPin->pin] |= (1U << 0);
}

// ================================================================================
// INTERRUPT STATUS FUNCTION
// ================================================================================

bool portCheckInterrupt(pin_handler_t *io_pin) {
	PORT_Type *pinPort = portGetPinPort(io_pin);
	return pinPort->ISFR & (1U << io_pin->pin);
}

void portClearInterrupt(pin_handler_t *io_pin) {
	PORT_Type *pinPort = portGetPinPort(io_pin);
	pinPort->ISFR |= (1U << io_pin->pin);
}

// ================================================================================
// PRIVATE FUNCTIONS IMPLEMENTATION
// ================================================================================

PORT_Type *portGetPinPort(pin_handler_t *io_pin) {
	if(!portPinValidate(io_pin)) {
		while(1);
	}
	return pin_ports[io_pin->port];
}

