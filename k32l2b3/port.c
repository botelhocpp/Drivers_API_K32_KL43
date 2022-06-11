/**
 * @file    port.c
 * @brief   Carries out all the control of the
 *          PORT peripheral.
 * @author  Pedro Botelho
 */

#include <k32_api/clock.h>
#include <k32_api/port.h>

#include "K32L2B31A.h"

#include <stdint.h>
#include <stdbool.h>


static PORT_Type *pin_ports[5] = {PORTA, PORTB, PORTC, PORTD, PORTE};

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

void portSetMuxGpio(pin_handler_t *io_pin) {
	if(!portPinValidate(io_pin)) {
		while(1);
	}
	PORT_Type *pinPort = pin_ports[io_pin->port];
	pinPort->PCR[io_pin->pin] &= ~(0b110 << 8);
	pinPort->PCR[io_pin->pin] |=  (0b001 << 8);
}

void portConfigInterrupt(pin_handler_t *inputPin, port_interrupt interrupt) {
	if(!portPinValidate(inputPin)) {
		while(1);
	}
	if(interrupt < portINT_LOGIC_0 || interrupt > portINT_LOGIC_1) {
		while(1);
	}
	PORT_Type *pinPort = pin_ports[inputPin->port];
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
	if(!portPinValidate(inputPin)) {
		while(1);
	}
	PORT_Type *pinPort = pin_ports[inputPin->port];
	pinPort->PCR[inputPin->pin] |= (1U << 1);
	pinPort->PCR[inputPin->pin] |= (1U << 0);
}

// ================================================================================
// INTERRUPT STATUS FUNCTION
// ================================================================================

bool portCheckInterrupt(pin_handler_t *io_pin) {
	if(!portPinValidate(io_pin)) {
		while(1);
	}
	PORT_Type *pinPort = pin_ports[io_pin->port];
	if(pinPort->PCR[io_pin->pin] & (1U << 24)) {
		pinPort->PCR[io_pin->pin] |= (1U << 24);
		return true;
	}
	return false;
}

