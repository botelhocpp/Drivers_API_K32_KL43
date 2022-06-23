#ifndef DRIVERS_H
#define DRIVERS_H

#include <k32l2b3/port.h>
#include <stdbool.h>

#define SW1_IRQHandler PORTA_IRQHandler
#define SW3_IRQHandler PORTC_PORTD_IRQHandler

typedef enum {
	drvINT_LOGIC_1 = portINT_LOGIC_0,
	drvINT_FALLING_EDGE = portINT_RISING_EDGE,
	drvINT_RISING_EDGE = portINT_FALLING_EDGE,
	drvINT_EITHER_EDGE = portINT_EITHER_EDGE,
	drvINT_LOGIC_0 = portINT_LOGIC_1
} drv_interrupt_event;

typedef enum {
	drvLED_SET,
	drvLED_CLEAR,
	drvLED_TOGGLE
} drv_led_function;

typedef enum {
	drvLED_GREEN,
	drvLED_RED
} drv_led_id;

typedef enum {
	drvBUTTON_SW1,
	drvBUTTON_SW3
} drv_button_id;

typedef enum {
	drvBUTTON_PRESSED,
	drvBUTTON_NOT_PRESSED
} drv_button_level;

typedef enum {
	drvLED_ON,
	drvLED_OFF
} drv_led_level;

/*!
 * @brief Initializes the CPU clock as 48 MHz.
 */
extern void drvCpuClockInit(void);

/*!
 * @brief Initializes the two on-board LEDS (green and red).
 */
extern void drvLedInit(void);

/*!
 * @brief Initializes the two on-board buttons (SW1 and SW3).
 */
extern void drvButtonInit(void);

/*!
 * @brief Sets the LED level based on a given function.
 * @param led The desired on-board LED ID.
 * @param function The desired function to perform.
 */
extern void drvLedSet(drv_led_id led, drv_led_function function);

/*!
 * @brief Checks the given on-board button level, via polling.
 * @param button The on-board button ID to check.
 * @retval "drvBUTTON_PRESSED" if the button have been pressed, or
 * "drvBUTTON_NOT_PRESSED" otherwise.
 */
extern drv_button_level drvCheckButtonLevel(drv_button_id button);

/*!
 * @brief Checks whether the given on-board button have been
 * pressed, via polling.
 * @param button The on-board button ID to check.
 * @retval "true" if the button have been pressed, of
 * "false" otherwise.
 */
extern bool drvCheckButtonPressed(drv_button_id button);

/*!
 * @brief Checks whether the given on-board button have been
 * pressed, via polling.
 * @param button The on-board button ID to enable interrupts.
 * @param int_event The interrupt event that triggers an IRQ.
 */
extern void drvButtonIntEnable(drv_button_id button, drv_interrupt_event int_event);

/*!
 * @brief Checks whether the button pin caused an interrupt.
 * @param button The on-board button ID to check for interrupts.
 * @retval "true" if an interrupt have occurred, and "false" otherwise.
 */
extern bool drvButtonIntCheck(drv_button_id button);

/*!
 * @brief Clears the interrupt flag in the button pin.
 * @param button The on-board button ID that caused the interrupts.
 */
extern void drvButtonIntClear(drv_button_id button);

#endif
