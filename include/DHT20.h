#include "hardware/i2c.h"
#include "pico/stdlib.h"
#include <inttypes.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// structure to store informatio about sensor
typedef struct DHT20 {
  // stores information regarding humidity
  float humidity;
  float humOffset;

  // stores information about sensor
  uint32_t lastRead;
  uint8_t crc;

  // stores data retrieved from sensor
  uint8_t bytes[7];
} DHT20;

// run this to initialize i2c channel and sensor values
int start_DHT20_sensor(DHT20 *sens);

// use this to retrieve stored humidity value
float get_humidity(DHT20 *sensor);

// use this to send signal to get measurement
// retrieve, convert and store that measurement
int take_measurement(DHT20 *sensor);