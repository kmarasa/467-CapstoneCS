/*
Functions to setup and run DHT20 sensor using
I2C protocol on GPIO 6 and 7.
Strong influence from:
https://github.com/sampsapenna/dht20-pico/tree/788e6020a24921907a98106239692eedc2d0cab3

Modifications include reduction and refactor.
Extra unnecessary functions were rewritten and slight
changes made to how the remaining functions run.
*/

#include "DHT20.h"

#include <pico/time.h>

/*
Error Codes
1 = Attempts at resetting sensor failed
2 = Not enough time elapsed between reading calls
3 = Pico generic error
4 = Data still generating by sensor
5 = all retrieved bytes are zero
6 = Checksum was not correct
100 = No clue what went wrong.
*/
static const int not_resetting = 1;
static const int have_patience = 2;
static const int pico_error = 3;
static const int still_measuring = 4;
static const int no_measurement = 5;
static const int incorrect_checksum = 6;
static const int pico_error_1 = 7;
static const int pico_error_2 = 8;
static const int cry_inside = 100;

// max amount of attempts to try to connect to sensor
static const int attempts = 5;

/*
Private function used to verify the bytes of
data sent by the sensor using CRC8 maxim
with initial value of 0xFF according to pg. 10
of DHT20 documentation.
Returns true(1) if match
*/
static int verify_checksum(DHT20 *sensor) {
  // sets initial value of crc
  uint8_t crc = 0xFF;
  // iterates through the bytes of data
  for (int i = 0; i < 6; i++) {
    // xors crc with the current byte
    crc = crc ^ sensor->bytes[i];
    // iterates through the digits in the byte
    for (int j = 0; j < 8; j++) {
      /*
      Checks if the current digit is 1 then
      shifts to the left and applies the polynomial
      function (as according to pg. 10) if the
      current digit is 1
      */
      if (crc & 0x80) {
        crc = (crc << 1) ^ 0x31;
      } else {
        crc = (crc << 1);
      }
    }
  }
  // last byte is the crc sent by the sensor
  return crc == sensor->bytes[6];
}

/*
Defined on pg. 10 of DHT20 Documentation
reset_i are messages to be sent to sensor to reset it.
request is the message to request sensor to take
a measurement.
*/
static const uint8_t reset_1[3] = {0x1B, 0x00, 0x00};
static const uint8_t reset_2[3] = {0x1C, 0x00, 0x00};
static const uint8_t reset_3[3] = {0x1E, 0x00, 0x00};
static const uint8_t request[3] = {0xAC, 0x33, 0x00};

// Define which i2c channel to use and address of DHT20
#ifndef DHT20_I2C
#define DHT20_I2C i2c1
#endif
#define DHT20_ADDRESS 0x38

// Define sda/scl pins for dht20 as GPIO 6/7
#ifndef DHT20_I2C_SDA_PIN
#define DHT20_I2C_SDA_PIN 6
#endif
#ifndef DHT20_I2C_SCL_PIN
#define DHT20_I2C_SCL_PIN 7
#endif

/*
Private function to initialize controller for i2c channel.
Uses defined value to set to channel i2c0 on address 0x38
using GPIO 6/7 as the SDA and SCL pins.
*/
static void set_DHT_controller() {
#ifndef DHT20_SKIP_INIT_SLEEP
  sleep_ms(2000);
#endif
  i2c_init(DHT20_I2C, 100000);
  gpio_set_function(DHT20_I2C_SDA_PIN, GPIO_FUNC_I2C);
  gpio_set_function(DHT20_I2C_SCL_PIN, GPIO_FUNC_I2C);
  gpio_pull_up(DHT20_I2C_SDA_PIN);
  gpio_pull_up(DHT20_I2C_SCL_PIN);
}

/*
Private function to initialize the values of the
DHT20 sensor object to zero.
*/
static void initialize_values(DHT20 *sensor) {
  sensor->humidity = 0;

  sensor->temperature = 0;

  sensor->lastRead = 0;

  memset(sensor->bytes, 0, 7);
}

/*
Private function to handle resetting sensor
Instructions in DHT20 sensor documentation pg. 10
Checks if status and 0x18 =/= 0x18 and sends
reset messages if not.
Returns 0 if success
*/
static int handle_reset(DHT20 *sensor) {
  uint8_t status;
  i2c_read_blocking(DHT20_I2C, DHT20_ADDRESS, &status, 1, false);
  for (int count = 0; count < attempts; count++) {
    if ((status & 0x18) != 0x18) {
      i2c_write_blocking(DHT20_I2C, DHT20_ADDRESS, reset_1, 3, false);
      i2c_write_blocking(DHT20_I2C, DHT20_ADDRESS, reset_2, 3, false);
      i2c_write_blocking(DHT20_I2C, DHT20_ADDRESS, reset_3, 3, false);
    } else if ((status & 0x18) == 0x18) {
      return 0;
    } else if (count == attempts - 1) {
      // I'm not editing as I have been requested not to
      // - but looks like this shouldn't be behind an
      // else if. Likely should be an if or a return
      // after the for loop has finished.
      return not_resetting;
    }
    sleep_ms(10);
    i2c_read_blocking(DHT20_I2C, DHT20_ADDRESS, &status, 1, false);
  }
}

/*
Public function to setup up DHT20 sensor
for its first time. Will create I2C controller
and initialize the values of the sensor.
Returns 0 if successful
*/
int start_DHT20_sensor(DHT20 *sensor) {
  // start controller
  set_DHT_controller();

  // initialize values of DHT20 object
  initialize_values(sensor);

  // may need to wait at least 100ms (pg. 10)
  // wait in controller start might work for this

  // reset sensor
  return handle_reset(sensor);
};

/*
Private function used to check if measurement
is ready and verify the contents.
Returns 0 if successful.
*/
static int retrieve_measure(DHT20 *sensor) {
  if (i2c_read_blocking(DHT20_I2C, DHT20_ADDRESS, sensor->bytes, 7, false) ==
      PICO_ERROR_GENERIC) {
    return pico_error;
  }

  // pg. 10 if status bit[7] == 0 measurement is complete
  if ((sensor->bytes[0] & 0x80) == 0x80) {
    return still_measuring;
  }

  // check to make sure bytes aren't all zero
  for (int i = 0; i < 6; i++) {
    if (sensor->bytes[i] > 0) {
      break;
    } else if (i == 5) {
      return no_measurement;
    }
  }

  sensor->lastRead = to_ms_since_boot(get_absolute_time());
  return 0;
}

/*
Private function that retrieves the data relevant to
the humidity value then converts it according to the
formula provided by pg. 11 in the DHT20 documentation.
*/
static int convert_humidity(DHT20 *sensor) {
  // according to pg. 10, humidity measurement
  // is store in the 1st, 2nd and 1st half of the 3rd byte
  uint32_t raw = sensor->bytes[1];
  raw <<= 8;
  raw += sensor->bytes[2];
  raw <<= 4;
  raw += (sensor->bytes[3] >> 4);
  sensor->humidity = raw / pow(2, 20) * 100;

  return 0;
}

/*
Private function that retrieves the data relevant to
the temperature value that converts it according to the
formula provided by pg. 11 in the DHT20 documentation.
*/
static int convert_temperature(DHT20 *sensor) {
  // stored in 2nd half of 3rd, 4th and 5th bytes
  uint32_t raw = sensor->bytes[3] & 0x0F;
  raw <<= 8;
  raw += sensor->bytes[4];
  raw <<= 8;
  raw += sensor->bytes[5];
  //converts to Fahrenheit
  sensor->temperature = (raw / pow(2, 20) * 200 - 50)*9/5+32;
  return 0;
}

/*
Public function that requests, retrieves and
processes measurement from the DHT20 sensor.
Return 0 if successful
*/
int take_measurement(DHT20 *sensor) {
  // check time since last measurement
  if (to_ms_since_boot(get_absolute_time()) - sensor->lastRead < 1000) {
    return have_patience;
  }

  // send message to dht20 to start measuring
  if (i2c_write_blocking(DHT20_I2C, DHT20_ADDRESS, request, 3, false) ==
      PICO_ERROR_GENERIC) {
    return pico_error_1;
  }

  // wait until dht20 has finished collecting measurement
  sleep_ms(80);
  for (int count = 0; count < attempts; count++) {
    if (!retrieve_measure(sensor)) {
      break;
    } else if (count == attempts - 1) {
      return pico_error_2;
    }
    sleep_ms(10);
  }

  // verify checksum
  if (!verify_checksum(sensor)) {
    return incorrect_checksum;
  }

  if(convert_humidity(sensor)){
    return 1;
  }
  if(convert_temperature(sensor)){
    return 1;
  }
  return 0;
}

/*
Public function to retrieve the humidity value store.
Returns last retrieved humidity value as float
*/
float get_humidity(DHT20 *sensor) { return sensor->humidity; }

/*
Public function to retrieve the temperature value stored.
Returns last retrieved temperature value as float
*/
float get_temperature(DHT20 *sensor) { return sensor->temperature; }
