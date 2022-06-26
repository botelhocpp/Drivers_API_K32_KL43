/**
 * @file    gpio.h
 * @brief   Carries out all the control of the
 *          GPIO peripheral, in the configuration
 *          and reading/writing of pins.
 * @author  Pedro Botelho
 */

#ifndef GPIO_H
#define GPIO_H

#include <k32l2b3/port.h>
#include "K32L2B31A.h"

// ================================================================================
// GPIO CONSTANTS
// ================================================================================

typedef enum {
	gpioINPUT,
	gpioOUTPUT
} gpio_direction;

typedef enum {
	gpioLOW,
	gpioHIGH
} gpio_level;

// ================================================================================
// FUNCTION PROTOTYPES
// ================================================================================

extern void gpioPinInit(pin_handler_t *io_pin, gpio_direction direction);

extern void gpioPinWrite(pin_handler_t *io_pin, gpio_level level);

extern void gpioPinToggle(pin_handler_t *io_pin);

extern gpio_level gpioPinRead(pin_handler_t *io_pin);

#endif
