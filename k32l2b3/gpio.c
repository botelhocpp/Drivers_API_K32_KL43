/**
 * @file    gpio.c
 * @brief   Carries out all the control of the
 *          GPIO peripheral, in the configuration
 *          and reading/writing of pins.
 * @author  Pedro Botelho
 */

#include <k32l2b3/gpio.h>
#include <k32l2b3/port.h>
#include "K32L2B31A.h"

#include <stdint.h>
#include <stdbool.h>

// ================================================================================
// GPIO ADDRESSES
// ================================================================================

static GPIO_Type *pin_gpios[5] = {GPIOA, GPIOB, GPIOC, GPIOD, GPIOE};

// ================================================================================
// GPIO FUNCTIONS
// ================================================================================

void gpioPinInit(pin_handler_t *io_pin, gpio_direction direction) {

	if(!portPinValidate(io_pin)) {
		while(1);
	}

	portInitPort(io_pin->port);
	portSetMuxGpio(io_pin);

	GPIO_Type *gpioPin = pin_gpios[io_pin->port];

	switch(direction) {
		case gpioINPUT:
			gpioPin->PDDR &= ~(1U << io_pin->pin);
			break;
		case gpioOUTPUT:
			gpioPin->PDDR |=  (1U << io_pin->pin);
			break;
		default:
			while(1);
	}
}

void gpioPinWrite(pin_handler_t *io_pin, gpio_level level) {
	if(!portPinValidate(io_pin)) {
		while(1);
	}

	GPIO_Type *gpioPin = pin_gpios[io_pin->port];

	switch(level) {
		case gpioLOW:
			gpioPin->PCOR |= (1U << io_pin->pin);
			break;
		case gpioHIGH:
			gpioPin->PSOR |= (1U << io_pin->pin);
			break;
		default:
			while(1);
	}
}

void gpioPinToggle(pin_handler_t *io_pin) {
	if(!portPinValidate(io_pin)) {
		while(1);
	}

	GPIO_Type *gpioPin = pin_gpios[io_pin->port];

	gpioPin->PTOR |= (1U << io_pin->pin);
}

gpio_level gpioPinRead(pin_handler_t *io_pin) {
	if(!portPinValidate(io_pin)) {
		while(1);
	}

	return pin_gpios[io_pin->port]->PDIR & (1U << io_pin->pin);
}
