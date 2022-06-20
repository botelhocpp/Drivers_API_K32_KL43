#ifndef LCD_H
#define LCD_H

#include "k32_api/port.h"

#define LCD_MODE_4_BITS 		1

typedef struct {
	#ifdef LCD_MODE_4_BITS
	pin_handler_t data[4];
	#else
	pin_handler_t data[8];
	#endif
	pin_handler_t rs;
	pin_handler_t en;
} lcd_handler_t;

extern void lcdInitModule(lcd_handler_t *lcd);

extern void lcdInitPinsModule(lcd_handler_t *lcd);

extern void lcdClearDisplay(lcd_handler_t *lcd);

extern void lcdSetCursor(lcd_handler_t *lcd, uint8_t row, uint8_t columm);

extern void lcdWriteString(lcd_handler_t *lcd, char *lcd_str);

extern void lcdWriteChar(lcd_handler_t *lcd, char lcd_char);

extern void lcdReturnCursor(lcd_handler_t *lcd);

extern void lcdScrollLeft(lcd_handler_t *lcd);

extern void lcdScrollRight(lcd_handler_t *lcd);


extern void lcdCreateChar(lcd_handler_t *lcd, uint8_t location, char *char_map);

extern void lcdCreateBigNumbers(lcd_handler_t *lcd);

extern void lcdWriteBigNumber(lcd_handler_t *lcd, uint8_t columm, uint8_t number);

#endif
