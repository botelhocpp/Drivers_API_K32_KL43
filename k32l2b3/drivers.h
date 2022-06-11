#ifndef DRIVERS_H
#define DRIVERS_H

typedef enum {
	DRV_LED_SET,
	DRV_LED_CLEAR,
	DRV_LED_TOGGLE
} drv_led_function;

typedef enum {
	DRV_LED_GREEN,
	DRV_LED_RED
} drv_led_id;

typedef enum {
	DRV_BUTTON_SW1,
	DRV_BUTTON_SW3
} drv_button_id;

typedef enum {
	DRV_BUTTON_PRESSED,
	DRV_BUTTON_NOT_PRESSED
} drv_button_level;

typedef enum {
	DRV_LED_ON,
	DRV_LED_OFF
} drv_led_level;

extern void drvCpuClockInit(void);

extern void drvLedInit(void);

extern void drvButtonInit(void);

extern void drvLedSet(drv_led_id led, drv_led_function function);

extern drv_button_level drvCheckButtonPressed(drv_button_id button);

extern void drvTestLeds(void);

extern void drvTestButtons(void);

#endif
