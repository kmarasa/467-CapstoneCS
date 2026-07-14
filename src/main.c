#include "../include/DHT20.h"

float humd = 0;
DHT20 humd_sensor;

void initialize()
{
    // start DHT20 sensor
    // will return 0 if successful
    start_DHT20_sensor(&humd_sensor);
}

int main()
{
    // initialize all the components
    initialize();

    while (1)
    {
        // get humidity from DHT20 sensor
        take_measurement(&humd_sensor)

            // store humidity from sensor
            humd = get_humidity(&humd_sensor);
    }
}