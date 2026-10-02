#ifndef _LCD_H_
#define _LCD_H_

#include <stdint.h>

void lcd_start(void);
void lcd_update_range(int16_t x1, int16_t y1, int16_t x2, int16_t y2);
void lcd_update_image(uint8_t* data, uint16_t size);

#endif // _LCD_H_