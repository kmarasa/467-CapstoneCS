/*
Six stage LED array driver for humidity monitor.
Maps humidity percentage level to number of LEDs lit.

@kmarasa
*/

#include "led_array.h"
#include "pico/stdlib.h"

#define LED_1 14 // Green 1 1-20% humidity
#define LED_2 15 // Green 2 > 20% humidity
#define LED_3 16 // Yellow 1 > 40% humidity
#define LED_4 17 // Yellow 2 > 60% humidity
#define LED_5 18 // Red 1 > 80% humidity

int led_pins[] = {LED_1, LED_2, LED_3, LED_4, LED_5};
int num_leds = 5;

/*
Initialized LEDs to their GPIO pins
*/
void init_leds() {
  for (int i = 0; i < num_leds; i++) {
    gpio_init(led_pins[i]);
    gpio_set_dir(led_pins[i], GPIO_OUT);
    gpio_put(led_pins[i], 0);
  }
}

/*
Test individual LEDs then test all together
*/
void test_leds() {
  for (int i = 0; i < num_leds; i++) {
    gpio_put(led_pins[i], 1);
    sleep_ms(1000);
    gpio_put(led_pins[i], 0);
  }
  sleep_ms(1000);
  for (int i = 0; i < num_leds; i++) {
    gpio_put(led_pins[i], 1);
  }
  sleep_ms(1000);

  for (int i = 0; i < num_leds; i++) {
    gpio_put(led_pins[i], 0);
  }
  sleep_ms(1000);
}

/*
Maps LEDs lit based on humidity percentage
*/
void set_leds(float humidity) {
  int leds_on = 0;
  if (humidity <= 1.00)
    leds_on = 0;
  if (humidity > 1.00)
    leds_on = 1;
  if (humidity > 20.00)
    leds_on = 2;
  if (humidity > 40.00)
    leds_on = 3;
  if (humidity > 60.00)
    leds_on = 4;
  if (humidity > 80.00)
    leds_on = 5;
  for (int i = 0; i < num_leds; i++) {
    gpio_put(led_pins[i], i < leds_on ? 1 : 0);
  }
}

void blink() {
  for (int i = 0; i < num_leds; i++) {
    gpio_put(led_pins[i], 1);
    sleep_ms(100);
    gpio_put(led_pins[i], 0);
  }
}
