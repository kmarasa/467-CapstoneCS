#ifndef LCD_H
#define LCD_H

#include <stdint.h>

void lcd_init();
void lcd_test();
void lcd_show_humidity(float humidity);
void lcd_show_temperature(float temperature);
void lcd_show_humidity_and_temperature(float humidity, float temperature);
void lcd_print_error(int error);

#endif