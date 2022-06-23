#include <k32l2b3/clock.h>
#include <k32l2b3/drivers.h>
#include <k32l2b3/gpio.h>
#include <k32l2b3/port.h>
#include <stdbool.h>

// =========================================================================
// PRIVATE STATIC VARIABLES
// =========================================================================

static pin_handler_t drv_led_green;
static pin_handler_t drv_led_red;

static pin_handler_t drv_button_sw1;
static pin_handler_t drv_button_sw3;

// =========================================================================
// PRIVATE FUNCTIONS PROTOTYPES
// =========================================================================

/*!
 * @brief Get the pin handler for the given LED ID.
 * @param led The desired on-board LED ID.
 * @retval The pin handler of the given LED.
 */
pin_handler_t *drvGetLed(drv_led_id led);

/*!
 * @brief Get the pin handler for the given button ID.
 * @param button The desired on-board button ID.
 * @retval The pin handler of the given button.
 */
pin_handler_t *drvGetButton(drv_button_id button);

// =========================================================================
// PUBLIC FUNCTIONS IMPLEMENTATIONS
// =========================================================================

/*!
 * @brief Initializes the CPU clock as 48 MHz.
 */
void drvCpuClockInit(void) {
	clkEnableClock(clkSOURCE_HIRCLK);
	clkSetMainClock(clkSOURCE_HIRCLK);
}

/*!
 * @brief Initializes the two on-board LEDS (green and red).
 */
void drvLedInit(void) {

	drv_led_green.port = pinPORT_D;
	drv_led_green.pin = 5;
	gpioPinInit(&drv_led_green, gpioOUTPUT);
	gpioPinWrite(&drv_led_green, drvLED_OFF);

	drv_led_red.port = pinPORT_E;
	drv_led_red.pin = 31;
	gpioPinInit(&drv_led_red, gpioOUTPUT);
	gpioPinWrite(&drv_led_red, drvLED_OFF);
}

/*!
 * @brief Initializes the two on-board buttons (SW1 and SW3).
 */
void drvButtonInit(void) {
	drv_button_sw1.port = pinPORT_A;
	drv_button_sw1.pin = 4;
	gpioPinInit(&drv_button_sw1, gpioINPUT);
	portConfigPullup(&drv_button_sw1);

	drv_button_sw3.port = pinPORT_C;
	drv_button_sw3.pin = 3;
	gpioPinInit(&drv_button_sw3, gpioINPUT);
	portConfigPullup(&drv_button_sw3);
}

/*!
 * @brief Sets the LED level based on a given function.
 * @param led The desired on-board LED ID.
 * @param function The desired function to perform.
 */
void drvLedSet(drv_led_id led, drv_led_function function) {
	pin_handler_t *led_handle = drvGetLed(led);
	switch(function) {
		case drvLED_SET:
			gpioPinWrite(led_handle, drvLED_ON);
			break;
		case drvLED_CLEAR:
			gpioPinWrite(led_handle, drvLED_OFF);
			break;
		case drvLED_TOGGLE:
			gpioPinToggle(led_handle);
			break;
		default:
			while(1);
	}
}

/*!
 * @brief Checks the given on-board button level, via polling.
 * @param button The on-board button ID to check.
 * @retval "drvBUTTON_PRESSED" if the button have been pressed, or
 * "drvBUTTON_NOT_PRESSED" otherwise.
 */
drv_button_level drvCheckButtonLevel(drv_button_id button) {
	pin_handler_t *button_handle = drvGetButton(button);
	return gpioPinRead(button_handle);
}

/*!
 * @brief Checks whether the given on-board button have been
 * pressed, via polling.
 * @param button The on-board button ID to check.
 * @retval "true" if the button have been pressed, of
 * "false" otherwise.
 */
bool drvCheckButtonPressed(drv_button_id button) {
	return !drvCheckButtonLevel(button);
}

/*!
 * @brief Checks whether the given on-board button have been
 * pressed, via polling.
 * @param button The on-board button ID to enable interrupts.
 * @param int_event The interrupt event that triggers an IRQ.
 */
void drvButtonIntEnable(drv_button_id button, drv_interrupt_event int_event) {
	if(int_event < drvINT_LOGIC_1 || int_event > drvINT_LOGIC_0) {
		while(1);
	}
	pin_handler_t *onboard_btn = drvGetButton(button);
	portConfigInterrupt(onboard_btn, int_event);
}

/*!
 * @brief Checks whether the button pin caused an interrupt.
 * @param button The on-board button ID to check for interrupts.
 * @retval "true" if an interrupt have occurred, and "false" otherwise.
 */
void drvButtonIntClear(drv_button_id button) {
	pin_handler_t *onboard_btn = drvGetButton(button);
	portClearInterrupt(onboard_btn);
}

/*!
 * @brief Clears the interrupt flag in the button pin.
 * @param button The on-board button ID that caused the interrupts.
 */
bool drvButtonIntCheck(drv_button_id button) {
	pin_handler_t *onboard_btn = drvGetButton(button);
	return portCheckInterrupt(onboard_btn);
}

// =========================================================================
// PRIVATE FUNCTIONS IMPLEMENTATIONS
// =========================================================================

/*!
 * @brief Get the pin handler for the given LED ID.
 * @param led The desired on-board LED ID.
 * @retval The pin handler of the given LED.
 */
pin_handler_t *drvGetLed(drv_led_id led) {
	if(led > drvLED_RED || led < drvLED_GREEN) {
		while(1);
	}
	return (led == drvLED_GREEN) ? &drv_led_green : &drv_led_red;
}

/*!
 * @brief Get the pin handler for the given button ID.
 * @param button The desired on-board button ID.
 * @retval The pin handler of the given button.
 */
pin_handler_t *drvGetButton(drv_button_id button) {
	if(button > drvBUTTON_SW3 || button < drvBUTTON_SW1) {
		while(1);
	}
	return (button == drvBUTTON_SW3) ? &drv_button_sw3 : &drv_button_sw1;
}
