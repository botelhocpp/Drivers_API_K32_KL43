#include "lcd.h"

#include "general_api/delay.h"
#include "k32_api/gpio.h"
#include "k32_api/clock.h"

// ==========================================================================
// LCD INSTRUCTION CODES
// ==========================================================================

#define DISPLAY_TURN_ON  					0x0C
#define DISPLAY_TURN_OFF 					0x08
#define DISPLAY_CLEAR				 		0x01

#define CURSOR_TURN_ON				 		0x0E
#define CURSOR_TURN_OFF				 		0x0C
#define CURSOR_SHIFT_LEFT				 	0x10
#define CURSOR_SHIFT_RIGHT				 	0x14
#define CURSOR_RETURN				 		0x02
#define CURSOR_BLINK				 		0x0D
#define CURSOR_ALT				 			0x0F

#define WRITE_SHIFT_CURSOR_LEFT 			0x04
#define WRITE_SHIFT_CURSOR_RIGHT 			0x06

#define SHIFT_MESSAGE_LEFT 					0x18
#define SHIFT_MESSAGE_RIGHT 				0x1C

#define DISPLAY_2L_5X7_4BIT					0x28
#define DISPLAY_2L_5X7_8BIT					0x38

#define CURSOR_START_FIRST_LINE				0x80
#define CURSOR_START_SECOND_LINE			0xC0

// ==========================================================================
// PRIVATE FUNCTIONS PROTOTYPES
// ==========================================================================

void lcdSendCommand(lcd_handler_t *lcd, uint8_t command);

void lcdSendData(lcd_handler_t *lcd, uint8_t data);

void lcdSendBits(lcd_handler_t *lcd, uint8_t byte);

void lcdEnablePulse(lcd_handler_t *lcd);

void delay_ms(uint32_t ms);

void delay_us(uint32_t us);

// ==========================================================================
// PUBLIC FUNCTIONS IMPLEMENTATION
// ==========================================================================

void lcdInitPinsModule(lcd_handler_t *lcd) {
	gpioPinInit(&lcd->data[0], gpioOUTPUT);
	gpioPinInit(&lcd->data[1], gpioOUTPUT);
	gpioPinInit(&lcd->data[2], gpioOUTPUT);
	gpioPinInit(&lcd->data[3], gpioOUTPUT);
	gpioPinInit(&lcd->rs, gpioOUTPUT);
	gpioPinInit(&lcd->en, gpioOUTPUT);
	lcdInitModule(lcd);
}

void lcdInitModule(lcd_handler_t *lcd) {
	// WAIT 30ms FOR LCD TO STABILIZE THE VOLTAGE
	delay_ms(30);

	lcdSendCommand(lcd, CURSOR_SHIFT_RIGHT);
	lcdSendCommand(lcd, CURSOR_RETURN);
	lcdSendCommand(lcd, DISPLAY_2L_5X7_4BIT);
	lcdSendCommand(lcd, DISPLAY_TURN_OFF);
	lcdSendCommand(lcd, DISPLAY_CLEAR);
	lcdSendCommand(lcd, CURSOR_TURN_OFF);
	lcdSendCommand(lcd, CURSOR_START_FIRST_LINE);
}

void lcdClearDisplay(lcd_handler_t *lcd) {
	lcdSendCommand(lcd, DISPLAY_CLEAR);
	lcdSendCommand(lcd, CURSOR_RETURN);
}

void lcdSetCursor(lcd_handler_t *lcd, uint8_t row, uint8_t columm) {
	if(row > 1 || row < 0) {
		while(1);
	}
	uint8_t cursor_add = (row == 0) ? CURSOR_START_FIRST_LINE : CURSOR_START_SECOND_LINE;
	lcdSendCommand(lcd, cursor_add + columm);
}

void lcdWriteString(lcd_handler_t *lcd, char *lcd_str) {
	for(int i = 0; lcd_str[i] != '\0'; i++) {
		lcdSendData(lcd, (unsigned int) lcd_str[i]);
	}
}

void lcdWriteChar(lcd_handler_t *lcd, char lcd_char) {
	lcdSendData(lcd, (unsigned int) lcd_char);
}

void lcdReturnCursor(lcd_handler_t *lcd) {
	lcdSendCommand(lcd, CURSOR_RETURN);
}

void lcdScrollLeft(lcd_handler_t *lcd) {
	lcdSendCommand(lcd, SHIFT_MESSAGE_LEFT);
}

void lcdScrollRight(lcd_handler_t *lcd) {
	lcdSendCommand(lcd, SHIFT_MESSAGE_RIGHT);
}

void lcdCreateChar(lcd_handler_t *lcd, uint8_t location, char *char_map) {

}

void lcdCreateBigNumbers(lcd_handler_t *lcd) {

}

void lcdWriteBigNumber(lcd_handler_t *lcd, uint8_t columm, uint8_t number) {

}

// ==========================================================================
// PRIVATE FUNCTIONS IMPLEMENTATIONS
// ==========================================================================

void lcdSendCommand(lcd_handler_t *lcd, uint8_t command) {
	// LOGIC 0 IN RS FOR COMMAND
	gpioPinWrite(&lcd->rs, gpioLOW);

	// SEND THE COMMAND VIA 4-BIT INTERFACE
	#ifdef LCD_MODE_4_BITS
	lcdSendBits(lcd, command >> 4);
	#endif
	lcdSendBits(lcd, command);

	// IF RETURN COMMAND OR CLEAR WAIT FOR LCD
	if (command < 4) {
		delay_ms(4);
	}
	else {
		delay_us(100);
	}
}

void lcdSendData(lcd_handler_t *lcd, uint8_t data) {
	// LOGIC 1 IN RS FOR DATA
	gpioPinWrite(&lcd->rs, gpioHIGH);

	// SEND THE DATA VIA 4-BIT INTERFACE
	#ifdef LCD_MODE_4_BITS
	lcdSendBits(lcd, data >> 4);
	#endif
	lcdSendBits(lcd, data);
}

void lcdSendBits(lcd_handler_t *lcd, uint8_t byte) {
	for(int i = 0; i < 4; i++) {
		gpioPinWrite(&lcd->data[i], ((byte >> i) & 1U));
	}
	lcdEnablePulse(lcd);
}

void lcdEnablePulse(lcd_handler_t *lcd) {
	// LOGIC 0 IN EN
	gpioPinWrite(&lcd->en, gpioLOW);

	delay_us(1);

	// LOGIC 1 IN EN
	gpioPinWrite(&lcd->en, gpioHIGH);

	// WAIT 1us
	delay_us(1);

	// LOGIC 0 IN EN
	gpioPinWrite(&lcd->en, gpioLOW);

	// WAIT 45us
	delay_us(100);
}
