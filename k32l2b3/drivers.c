#include <k32_api/clock.h>
#include <k32_api/drivers.h>
#include <k32_api/gpio.h>
#include <k32_api/port.h>
#include <stdbool.h>

static pin_handler_t drv_led_green;
static pin_handler_t drv_led_red;

static pin_handler_t drv_button_sw1;
static pin_handler_t drv_button_sw3;

long drvCpuClockCounts() {
	switch(clkGetMainClock()) {
		case 48000000L:
			return 4000L;
		// TODO
	}
	return 0L;
}

/*!
 * @brief Inits the clock of the processor at 48 MHz.
 */
void drvCpuClockInit(void) {
	clkEnableClock(clkSOURCE_HIRCLK);
	clkSetMainClock(clkSOURCE_HIRCLK);
}

/*!
 * @brief Inits the two embedded LEDs of the board.
 */
void drvLedInit(void) {
	drv_led_green.port = pinPORT_D;
	drv_led_green.pin = 5;
	gpioPinInit(&drv_led_green, gpioOUTPUT);
	gpioPinWrite(&drv_led_green, DRV_LED_OFF);

	drv_led_red.port = pinPORT_E;
	drv_led_red.pin = 31;
	gpioPinInit(&drv_led_red, gpioOUTPUT);
	gpioPinWrite(&drv_led_red, DRV_LED_OFF);
}

/*!
 * @brief Inits the two embedded buttons of the board.
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
 * @brief Performs a given function with the given LED.
 */
void drvLedSet(drv_led_id led, drv_led_function function) {
	if(led > DRV_LED_RED || led < DRV_LED_GREEN) {
		while(1);
	}
	pin_handler_t *led_handle = (led == DRV_LED_GREEN) ? &drv_led_green : &drv_led_red;
	switch(function) {
		case DRV_LED_SET:
			gpioPinWrite(led_handle, DRV_LED_ON);
			break;
		case DRV_LED_CLEAR:
			gpioPinWrite(led_handle, DRV_LED_OFF);
			break;
		case DRV_LED_TOGGLE:
			gpioPinToggle(led_handle);
			break;
	}
}

drv_button_level drvCheckButtonPressed(drv_button_id button) {
	if(button > DRV_BUTTON_SW3 || button < DRV_BUTTON_SW1) {
		while(1);
	}
	pin_handler_t *button_handle = (button == DRV_BUTTON_SW1) ? &drv_button_sw1 : &drv_button_sw3;
	return gpioPinRead(button_handle);
}

void drvTestLeds(void) {
	for(int i = 0; i < 10; i++) {
		drvLedSet(DRV_LED_RED, DRV_LED_SET);
		drvLedSet(DRV_LED_GREEN, DRV_LED_SET);
		for(int j = 0; j < 100*drvCpuClockCounts(); j++);

		drvLedSet(DRV_LED_RED, DRV_LED_CLEAR);
		drvLedSet(DRV_LED_GREEN, DRV_LED_CLEAR);
		for(int j = 0; j < 100*drvCpuClockCounts(); j++);
	}
}

void drvTestButtons(void) {
	bool sw1_double_tap = false;
	bool sw3_double_tap = false;
	while(1) {
		if(drvCheckButtonPressed(DRV_BUTTON_SW1) == DRV_BUTTON_PRESSED) {
			if(!sw1_double_tap) {
				sw1_double_tap = true;
				sw3_double_tap = false;
				drvLedSet(DRV_LED_GREEN, DRV_LED_TOGGLE);
			}
			else {
				break;
			}
		}
		if(drvCheckButtonPressed(DRV_BUTTON_SW3) == DRV_BUTTON_PRESSED) {
			if(!sw3_double_tap) {
				sw3_double_tap = true;
				sw1_double_tap = false;
				drvLedSet(DRV_LED_RED, DRV_LED_TOGGLE);
			}
			else {
				break;
			}
		}
	}
	drvLedSet(DRV_LED_GREEN, DRV_LED_CLEAR);
	drvLedSet(DRV_LED_RED, DRV_LED_CLEAR);
}
