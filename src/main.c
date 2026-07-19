#include "../include/DHT20.h"
#include "../include/led_array.h"
#include "../include/lcd.h"

float humidity = 0;
DHT20 humidity_sensor;

/*
 *Run setup for main program
 */
void initialize() {
  /*
   *Initialize DHT20 sensor on GPIO 4/5 with address 0x38
   *Will also set initial values on DHT20 object
   *Returns 0 if successful
   */
  start_DHT20_sensor(&humidity_sensor);

  init_leds();

  test_leds();

  lcd_init();

  lcd_test();
}

/*
Runs main program loop after running setup
*/
int main() {
  // initialize all the components
  initialize();

  int error = 0;
  while (1) {
    // get humidity from DHT20 sensor
    error = take_measurement(&humidity_sensor);
    if (error == 0) {
      // store humidity from sensor
      humidity = get_humidity(&humidity_sensor);

      set_leds(humidity);
      lcd_show_humidity(humidity);
    } else {
      lcd_print_error(error);
    }
    sleep_ms(1000);
  }
}