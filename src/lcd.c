#include "lcd.h"
#include "hardware/i2c.h"
#include "pico/stdlib.h"
#include <stdio.h>

#ifndef LCD_I2C
#define LCD_I2C i2c0
#endif

#ifndef LCD_I2C_SDA_PIN
#define LCD_I2C_SDA_PIN 4
#endif

#ifndef LCD_I2C_SCL_PIN
#define LCD_I2C_SCL_PIN 5
#endif

#define LCD_ADDRESS 0x27
#define LCD_COLS 16
#define I2C_BAUD_RATE 50000

#define LCD_RS 0x01
#define LCD_EN 0x04
#define LCD_BACKLIGHT 0x08

static void lcd_write_raw(uint8_t data) {
  i2c_write_blocking(LCD_I2C, LCD_ADDRESS, &data, 1, false);
}

static void lcd_pulse_enable(uint8_t data) {
  lcd_write_raw(data | LCD_EN);
  sleep_us(1);
  lcd_write_raw(data & ~LCD_EN);
  sleep_us(50);
}

static void lcd_write_nibble(uint8_t nibble, uint8_t rs) {
  uint8_t data = ((nibble & 0x0F) << 4) | LCD_BACKLIGHT;
  if (rs) {
    data |= LCD_RS;
  }
  lcd_pulse_enable(data);
}

static void lcd_send_byte(uint8_t value, uint8_t rs) {
  lcd_write_nibble(value >> 4, rs);
  lcd_write_nibble(value & 0x0F, rs);
}

static void lcd_command(uint8_t command) {
  lcd_send_byte(command, 0);
  if (command == 0x01 || command == 0x02) {
    sleep_ms(2);
  }
}

void lcd_init(void) {
  i2c_init(LCD_I2C, I2C_BAUD_RATE);
  gpio_set_function(LCD_I2C_SDA_PIN, GPIO_FUNC_I2C);
  gpio_set_function(LCD_I2C_SCL_PIN, GPIO_FUNC_I2C);
  gpio_pull_up(LCD_I2C_SDA_PIN);
  gpio_pull_up(LCD_I2C_SCL_PIN);

  sleep_ms(50);
  lcd_write_nibble(0x03, 0);
  sleep_ms(5);
  lcd_write_nibble(0x03, 0);
  sleep_us(150);
  lcd_write_nibble(0x03, 0);
  lcd_write_nibble(0x02, 0);

  lcd_command(0x28); // 4-bit mode, 2 lines, 5x8 font
  lcd_command(0x08); // display off
  lcd_command(0x01); // display clear
  lcd_command(0x06); // entry mode set
  lcd_command(0x0C); // display on, cursor off
}

void lcd_test(void) {
  lcd_clear();
  lcd_set_cursor(0, 0);
  lcd_print("LCD Test");
  lcd_set_cursor(0, 1);
  lcd_print("Humidity Monitor");
  sleep_ms(1200);
  lcd_clear();
}

void lcd_clear(void) { lcd_command(0x01); }

void lcd_set_cursor(uint8_t col, uint8_t row) {
  static const uint8_t row_offsets[] = {0x00, 0x40, 0x14, 0x54};
  if (row > 1) {
    row = 1;
  }
  if (col >= LCD_COLS) {
    col = LCD_COLS - 1;
  }
  lcd_command(0x80 | (col + row_offsets[row]));
}

void lcd_print(const char *text) {
  while (*text) {
    lcd_send_byte((uint8_t)*text, 1);
    text++;
  }
}

void lcd_show_humidity(float humidity) {
  char line[17];

  lcd_set_cursor(0, 0);
  lcd_print("Humidity        ");

  snprintf(line, sizeof(line), "%5.1f%% RH       ", humidity);
  lcd_set_cursor(0, 1);
  lcd_print(line);
}

void lcd_print_error(int error) {
  lcd_set_cursor(0, 0);
  lcd_print("DHT20 Error Code");

  char line[17];

  snprintf(line, sizeof(line), "%-16d", error);
  lcd_set_cursor(0, 1);
  lcd_print(line);
}