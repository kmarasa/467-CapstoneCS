#include "DHT20.h"

// define error codes for sensor
// not_resetting means tried to run reset codes
// 5 times and sensor is still not resetting
static const int not_resetting = 1;
// have_patience means trying to call for another
// reading too quickly. slow down.
static const int have_patience = 2;
// pico generic error
static const int pico_error = 3;
// still retrieving data wait and try again
static const int still_measuring = 4;
// after retrieving the measurement
// all bytes are still zero
static const int no_measurement = 5;
// checksum is not correct
static const int incorrect_checksum = 6;
// general error code for all other cases
// made large for extra space for more errors
static const int cry_inside = 100;

// max amount of attempts to try to connect to sensor
static const int attempts = 5;

// messages defined on pg. 10 in DHT20 documentation
static const uint8_t reset_1[3] = {0x1B, 0x00, 0x00};
static const uint8_t reset_2[3] = {0x1C, 0x00, 0x00};
static const uint8_t reset_3[3] = {0x1E, 0x00, 0x00};
static const uint8_t request[3] = {0xAC, 0x33, 0x00};

// define which i2c channel to use and address of DHT20
#ifndef DHT20_I2C
#define DHT20_I2C i2c0
#endif
#define DHT20_ADDRESS 0x38

// define sda/scl pins for dht20 as GPIO 4/5
#ifndef DHT20_I2C_SDA_PIN
#define DHT20_I2C_SDA_PIN 4
#endif
#ifndef DHT20_I2C_SCL_PIN
#define DHT20_I2C_SCL_PIN 5
#endif

// initialize controller for i2c channel
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

// returns 0 if status and 0x18 =/= 0x18
// in documentation for DHT20 sensor (pg. 10?)
static int needs_reset(DHT20 *sensor) {
  uint8_t status;
  i2c_read_blocking(DHT20_I2C, DHT20_ADDRESS, &status, 1, false);
  return (status & 0x18) != 0x18;
}

static void reset_sensor(DHT20 *sensor) {
  // sends reset codes defined in documentation
  i2c_write_blocking(DHT20_I2C, DHT20_ADDRESS, reset_1, 3, false);
  i2c_write_blocking(DHT20_I2C, DHT20_ADDRESS, reset_2, 3, false);
  i2c_write_blocking(DHT20_I2C, DHT20_ADDRESS, reset_3, 3, false);
}

static void initialize_values(DHT20 *sensor) {
  sensor->humidity = 0;
  sensor->humOffset = 0;

  sensor->lastRead = 0;
  sensor->crc = 0;

  memset(sensor->bytes, 0, 7);
}

int start_DHT20_sensor(DHT20 *sensor) {
  // start controller
  set_DHT_controller();

  // initialize values of DHT20 object
  initialize_values(sensor);

  // may need to wait at least 100ms (pg. 10)
  // wait in controller start might work for this

  // reset sensor
  int count = 0;
  while (!needs_reset(sensor)) {
    reset_sensor(sensor);
    count++;
    sleep_ms(100);
    if (count > attempts) {
      return not_resetting;
    }
  }
  return 0;
};

float get_humidity(DHT20 *sensor) { return sensor->humidity; }

static int retrieve_measure(DHT20 *sensor) {
  int result =
      i2c_read_blocking(DHT20_I2C, DHT20_ADDRESS, sensor->bytes, 7, false);
  if (result == PICO_ERROR_GENERIC) {
    return pico_error;
  }

  // pg. 10 if status bit[7] == 0 measurement is complete
  if ((sensor->bytes[0] & 0x80) == 0x80) {
    return still_measuring;
  }

  // check to make sure bytes aren't all zero
  int sum = 0;
  for (int i = 0; i < 7; i++) {
    sum = sum + sensor->bytes[i];
  }
  if (sum) {
    return no_measurement;
  }

  sensor->lastRead = to_ms_since_boot(get_absolute_time());
  return 0;
}

static int verify_checksum(DHT20 *sensor) {
  // still needs implemented.
  // information about crc in on pg. 10 #4
  return 0;
}

static int convert_humidity(DHT20 *sensor) {
  // according to pg. 10, humidity measurement
  // is store in the 1st, 2nd and 1st half of the 3rd byte
  uint32_t raw = sensor->bytes[1];
  raw <<= 8;
  raw += sensor->bytes[2];
  raw <<= 4;
  raw += (sensor->bytes[3] >> 4);
  sensor->humidity = raw / pow(2, 20) * 100;

  // verify checksum
  if (!verify_checksum(sensor)) {
    return incorrect_checksum;
  }

  return 0;
}

int take_measurement(DHT20 *sensor) {
  // check time since last measurement
  if (to_ms_since_boot(get_absolute_time()) - sensor->lastRead < 1000) {
    return have_patience;
  }

  // send message to dht20 to start measuring
  //  PICO_ERROR_GENERIC is a failure code is SDK (-2)
  int trigger_measure =
      i2c_write_blocking(DHT20_I2C, DHT20_ADDRESS, request, 3, false);
  if (trigger_measure == PICO_ERROR_GENERIC) {
    return pico_error;
  }

  // wait until dht20 has finished collecting measurement
  sleep_ms(80);
  // wait 80 ms for measurement (pg. 10?)
  // if read status bit[7] = 0, measurement is complete
  int completed = retrieve_measure(sensor);
  int count = 0;
  // may be able to check if completed == still_measuring
  while (!completed) {
    sleep_ms(10);
    completed = retrieve_measure(sensor);
    count++;
    if (count > attempts) {
      return pico_error;
    }
  }

  // convert humidity using formula on pg. 11
  convert_humidity(sensor);

  return 0;
}