/*
adapted via raspberry pi pico examples repo:
https://github.com/raspberrypi/pico-examples

converter for rgb
https://docs.sunfounder.com/projects/kepler-kit/en/latest/video_course/lesson34.html


and LAFVIN tutorials:
https://www.dropbox.com/scl/fo/ynihrh5p4qcsh5mief1en/h?dl=0&e=1&file_subpath=%2FC%2FCode%2FProject_4_RGB_LED&preview=Raspberry+Pi+Pico+Starter+Kit.zip&rlkey=v442lp2b2dvcct0bhmk0idlgb
https://www.dropbox.com/scl/fo/ynihrh5p4qcsh5mief1en/h?dl=0&e=1&file_subpath=%2FC%2FCode%2FProject_3_PWM_Control_LED&preview=Raspberry+Pi+Pico+Starter+Kit.zip&rlkey=v442lp2b2dvcct0bhmk0idlgb

*/

#include "led_temp.h"
#include "hardware/pwm.h"
#include <math.h>
// GPIO pins to RGB LED's color channels, these pins support pwm 
// pwm helps create a "dimmer" brightness level to help represent temperature
#define RGB_RED_PIN 10
#define RGB_GREEN_PIN 11
#define RGB_BLUE_PIN 12

// made to represent a basic range of temperature, not too specific
#define TEMP_MIN_F 32.0F
#define TEMP_MAX_F 100.0F

// pwm slice and channel numbers for each pin, essentially mapping dimmer hardware to pin
static uint slice_red, slice_green, slice_blue;
static uint chan_red, chan_green, chan_blue;

void init_rgb_led() {
  // route each gpios hardware function to pwm (initializes dimming functionality)
  gpio_set_function(RGB_RED_PIN, GPIO_FUNC_PWM);
  gpio_set_function(RGB_GREEN_PIN, GPIO_FUNC_PWM);
  gpio_set_function(RGB_BLUE_PIN, GPIO_FUNC_PWM);

  // dimming is now enabled, id internal dimmer circuit for each one
  slice_red = pwm_gpio_to_slice_num(RGB_RED_PIN);
  slice_green = pwm_gpio_to_slice_num(RGB_GREEN_PIN);
  slice_blue = pwm_gpio_to_slice_num(RGB_BLUE_PIN);

  chan_red = pwm_gpio_to_channel(RGB_RED_PIN);
  chan_green = pwm_gpio_to_channel(RGB_GREEN_PIN);
  chan_blue = pwm_gpio_to_channel(RGB_BLUE_PIN);

  // wrap val of 255 makes pwm counter count 0 - 255
  // this matched an 8 bit color channel ( 0 - 255) one to one
  // 0 = off, 255 = full brightness

  pwm_set_wrap(slice_red, 255);
  pwm_set_wrap(slice_green, 255);
  pwm_set_wrap(slice_blue, 255);

  // initialize LED to respond immediately after we initialize a brightness level
  pwm_set_enabled(slice_red, true);
  pwm_set_enabled(slice_green, true);
  pwm_set_enabled(slice_blue, true);
}

// converts hue angle to rgb using standard hsv-to-rgb formula
// (how much of each color is needed to make a specific color)
static void hue_to_rgb(float hue, uint8_t *red, uint8_t *green, uint8_t *blue) {
  float c = 255.0f; // initialize to full brightness

  // calculates value to smoothly blend between different colors 
  float x = c * (1.0f - fabsf(fmodf(hue / 60.0f, 2.0f) - 1.0f));

  float rf, gf, bf;

  // split color wheel into slices (red, yellow, green, cyan, blue, purple)
  // red mix with yello
  if (hue < 60) {
    rf = c;
    gf = x;
    bf = 0;
  // yellow mix with green 
  } else if (hue < 120) {
    rf = x;
    gf = c;
    bf = 0;
  // green mix with cyan
  } else if (hue < 180) {
    rf = 0;
    gf = c;
    bf = x;
  // cyan mix with blue
  } else if (hue < 240) {
    rf = 0;
    gf = x;
    bf = c;
  // blue mix with purple
  } else if (hue < 300) {
    rf = x;
    gf = 0;
    bf = c;
  // purple mix with red
  } else {
    rf = c;
    gf = 0;
    bf = x;
  }
  // round down to whole nums 0 - 255
  *red = (uint8_t)rf;
  *green = (uint8_t)gf;
  *blue = (uint8_t)bf;
}

void set_rgb_led_temp(float temp_f) {
// edge cases - if temp below 32 or above 100, display color of 32 or 100 respectively
  if (temp_f < TEMP_MIN_F)
    temp_f = TEMP_MIN_F;
  if (temp_f > TEMP_MAX_F)
    temp_f = TEMP_MAX_F;
// convert temp into fraction where = coldest, 1 = hottest

  float t = (temp_f - TEMP_MIN_F) / (TEMP_MAX_F - TEMP_MIN_F);
  
  // map fraction to color wheel 
  // the colder it is, the more blue it'll be (closer to 240)
  // hotter it is, the more red it'll be (closer to 0)
  float hue = 240.0f - (t * 240.0f);

  uint8_t r, g, b;
  hue_to_rgb(hue, &r, &g, &b);

  // set each colors brightness on physical LED
  pwm_set_chan_level(slice_red, chan_red, r);
  pwm_set_chan_level(slice_green, chan_green, g);
  pwm_set_chan_level(slice_blue, chan_blue, b);
}
