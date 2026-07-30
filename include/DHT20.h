/*
Header file for DHT20.c
Contains public functions, all libraries
and APIs that are used for DHT20 sensor.
Define DHT20 object to store and be used
to run the DHT20 sensor.
*/

#include "hardware/i2c.h"
#include "pico/stdlib.h"
#include <inttypes.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/*
Defines DHT20 object that will store information
about the sensor.
humidity (float) stores the last retrieved humidity value
lastRead (uint32_t) stores the time the last humidity value was retrieved
crc (uint8_t) stores the checksum to verify the data from the sensor
bytes (uint8_t) stores the last retrieved set of bytes
*/
typedef struct DHT20 {
  float humidity;
  float temperature;
  double last_measurement_time;
  uint32_t lastRead;
  uint8_t bytes[7];
} DHT20;

/*
Public function to setup up DHT20 sensor
for its first time. Will create I2C controller
and initialize the values of the sensor.
Returns 0 if successful initialized
Returns 1 if reset sensor fails
*/
int start_DHT20_sensor(DHT20 *sensor);

/*
Public function that requests, retrieves and
processes measurement from the DHT20 sensor.
Return 0 if successful
Returns 2 if more time before next measurement request is necessary
Returns 3 if there was a pico error when retrieving results
Returns 4 if measurement is not complete after 5 attempts
Returns 5 if measurement bytes were empty after 5 attempts
Returns 6 if checksum does not match
Returns 7 if error with pico when writing request to measure
*/
int take_measurement(DHT20 *sensor);

/*
Public function to retrieve the humidity value store.
Returns last retrieved humidity value as float
*/
float get_humidity(DHT20 *sensor);

/*
Public function to retrieve the temperature value stored.
Returns last retrieved temperature value as float
*/
float get_temperature(DHT20 *sensor);

/*
Public function to retrieve time required for last measurement.
Return time as float.
*/
float get_measurement_time(DHT20 *sensor);