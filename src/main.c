#include "../include/DHT20.h"
#include "../include/led_array.h"

float humd = 0;
DHT20 humd_sensor;

/*
 *Run setup for main program
 */
void initialize() {
  /*
   *Initialize DHT20 sensor on GPIO 4/5 with address 0x38
   *Will also set initial values on DHT20 object
   *Returns 0 if successful
   */
  start_DHT20_sensor(&humd_sensor);

  init_leds();

  test_leds();
}

/*
Runs main program loop after running setup
*/
int main() {
  // initialize all the components
  initialize();

  while (1) {
    // get humidity from DHT20 sensor
    if (take_measurement(&humd_sensor) == 0) {
      // store humidity from sensor
      humd = get_humidity(&humd_sensor);

      set_leds(humd);
    }
  }
}