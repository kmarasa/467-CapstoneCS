#include "../include/DHT20.h"
#include "../include/lcd.h"
#include "../include/led_array.h"
#include "../include/led_temp.h"

float humidity = 0;
float temperature = 0;
DHT20 humidity_sensor;

/*
 *Run setup for main program
 */
int initialize() {
  /*
   *Initialize DHT20 sensor on GPIO 4/5 with address 0x38
   *Will also set initial values on DHT20 object
   *Returns 0 if successful
   */
  int err = start_DHT20_sensor(&humidity_sensor);

  init_leds();

  test_leds();

  init_rgb_led();

  lcd_init();

  lcd_test();

  return err;
}

/*
Runs main program loop after running setup
*/
int main() {
  // initialize all the components
  int error = initialize();

  // if there is an initialization error, 
  // show the error and stop the program
  if (error != 0) {
    lcd_print_init_error(error);
    return 0;
  }

  while (1) {
    // get humidity and temperature from DHT20 sensor
    error = take_measurement(&humidity_sensor);
    if (error == 0) {
      // store humidity and temperature from sensor
      humidity = get_humidity(&humidity_sensor);
      temperature = get_temperature(&humidity_sensor);
      // set humidity and temperature leds
      set_leds(humidity);
      set_rgb_led_temp(temperature);
      // show humidity and temperature on the lcd screen
      lcd_show_humidity_and_temperature(humidity, temperature);

      // optional views; commented out in favor of viewing humidity and
      // temperature in one readout

      // lcd_show_humidity(humidity);
      // sleep_ms(2000);
      // // wait 2 seconds to allow for screen to be read
      // lcd_show_temperature(temperature);
      // sleep_ms(2000);
    } else {
      // if there is an error taking the reading, show the error
      lcd_print_error(error);
    }
    // wait 2 seconds between each reading
    sleep_ms(2000);
  }
}