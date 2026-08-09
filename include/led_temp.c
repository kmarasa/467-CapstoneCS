#include "led_temp.h"
#include "hardware/pwm.h"
#include <math.h>

#define RGB_RED_PIN 10
#define RGB_GREEN_PIN 11
#define RGB_BLUE_PIN 12

#define TEMP_MIN_F 32.0F
#define TEMP_MAX_F 100.0F

static uint slice_red, slice_green, slice_blue;
static uint chan_red, chan_green, chan_blue;

void init_rgb_led() {
  gpio_set_function(RGB_RED_PIN, GPIO_FUNC_PWM);
  gpio_set_function(RGB_GREEN_PIN, GPIO_FUNC_PWM);
  gpio_set_function(RGB_BLUE_PIN, GPIO_FUNC_PWM);

  slice_red = pwm_gpio_to_slice_num(RGB_RED_PIN);
  slice_green = pwm_gpio_to_slice_num(RGB_GREEN_PIN);
  slice_blue = pwm_gpio_to_slice_num(RGB_BLUE_PIN);

  chan_red = pwm_gpio_to_channel(RGB_RED_PIN);
  chan_green = pwm_gpio_to_channel(RGB_GREEN_PIN);
  chan_blue = pwm_gpio_to_channel(RGB_BLUE_PIN);

  pwm_set_wrap(slice_red, 255);
  pwm_set_wrap(slice_green, 255);
  pwm_set_wrap(slice_blue, 255);

  pwm_set_enabled(slice_red, true);
  pwm_set_enabled(slice_green, true);
  pwm_set_enabled(slice_blue, true);
}

static void hue_to_rgb(float hue, uint8_t *red, uint8_t *green, uint8_t *blue) {
  float c = 255.0f;
  float x = c * (1.0f - fabsf(fmodf(hue / 60.0f, 2.0f) - 1.0f));

  float rf, gf, bf;

  if (hue < 60) {
    rf = c;
    gf = x;
    bf = 0;
  } else if (hue < 120) {
    rf = x;
    gf = c;
    bf = 0;
  } else if (hue < 180) {
    rf = 0;
    gf = c;
    bf = x;
  } else if (hue < 240) {
    rf = 0;
    gf = x;
    bf = c;
  } else if (hue < 300) {
    rf = x;
    gf = 0;
    bf = c;
  } else {
    rf = c;
    gf = 0;
    bf = x;
  }

  *red = (uint8_t)rf;
  *green = (uint8_t)gf;
  *blue = (uint8_t)bf;
}

void set_rgb_led_temp(float temp_f) {
  if (temp_f < TEMP_MIN_F)
    temp_f = TEMP_MIN_F;
  if (temp_f > TEMP_MAX_F)
    temp_f = TEMP_MAX_F;

  float t = (temp_f - TEMP_MIN_F) / (TEMP_MAX_F - TEMP_MIN_F);
  float hue = 240.0f - (t * 240.0f);

  uint8_t r, g, b;
  hue_to_rgb(hue, &r, &g, &b);

  pwm_set_chan_level(slice_red, chan_red, r);
  pwm_set_chan_level(slice_green, chan_green, g);
  pwm_set_chan_level(slice_blue, chan_blue, b);
}
