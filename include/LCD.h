#ifndef LCD_H
#define LCD_H

#include <stdint.h>
#include "hardware/i2c.h"
#include "pico/stdlib.h"
#include <stdio.h>

void lcd_init(void);
void lcd_test(void);
void lcd_clear(void);
void lcd_set_cursor(uint8_t col, uint8_t row);
void lcd_print(const char *text);
void lcd_show_humidity(float humidity);
void lcd_print_error(int error);

#endif