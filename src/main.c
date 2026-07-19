#include "../include/DHT20.h"
#include "../include/lcd.h"
#include "../include/led_array.h"

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

  lcd_init();
  
  lcd_test();

  init_leds();

  test_leds();
}

/*
Runs main program loop after running setup
*/
int main() {
  // initialize all the components
  initialize();

  int DHT20Err;

  while (1) {
    DHT20Err = take_measurement(&humidity_sensor);
    // // get humidity from DHT20 sensor
    if (DHT20Err == 0) {
      // store humidity from sensor
      humidity = get_humidity(&humidity_sensor);

      set_leds(humidity);
      lcd_show_humidity(humidity);
    } else if (DHT20Err == 2){
      sleep_ms(1000);
    } else {
      blink();
      lcd_print_error(DHT20Err);      
    }
    sleep_ms(5000);
  }
}