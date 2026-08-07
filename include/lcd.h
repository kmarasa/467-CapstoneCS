#ifndef LCD_H
#define LCD_H

#include <stdint.h>

void lcd_init();
void lcd_test();
void lcd_show_humidity(float humidity);
void lcd_print_error(int error);

#endif