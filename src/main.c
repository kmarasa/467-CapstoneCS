#include "../include/DHT20.h"
#include "../include/lcd.h"
#include "../include/led_array.h"

float humidity = 0;
float temperature = 0;
DHT20 humidity_sensor;

//used for calculating average time of run
int count = 0;
double average_time = 0;

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

  if (error != 0) {
    lcd_print_init_error(error);
    return 0;
  }

  while (1) {
    // get humidity from DHT20 sensor
    error = take_measurement(&humidity_sensor);
    if (error == 0) {
      // increase count of measurements taken and add new time
      count++;
      average_time += get_measurement_time(&humidity_sensor);

      // store humidity from sensor
      humidity = get_humidity(&humidity_sensor);
      temperature = get_temperature(&humidity_sensor);
      set_leds(humidity);
      lcd_show_humidity(humidity);
      sleep_ms(2000); // wait 2 seconds to allow for screen to be read
      lcd_show_temperature(temperature);
      sleep_ms(2000);
      lcd_show_humidity_and_temperature(humidity, temperature);
    } else {
      lcd_print_error(error);
    }

    sleep_ms(2000);

    // when count reaches 10, calculate average
    // and print then reset
    if (count == 10) {
      average_time /= count;
      //insert print to lcd here
      count = 0;
      average_time = 0;
      sleep_ms(2000);
    }
  }
}