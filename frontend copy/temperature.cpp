#include <Arduino.h>
#include <DHT.h>
#include "temperature.h"

/* CONSTANTS */
#define DHT_PIN 4
#define DHT_TYPE DHT11

const float TEMP_THRESHOLD = 30.0;
const unsigned long TEMP_INTERVAL = 2000;

/* VARIABLES */
DHT dht(DHT_PIN, DHT_TYPE);

unsigned long last_temp_read = 0;

/* INITIALIZATION */
void temperature_init() {
    Serial.begin(9600);
    dht.begin();
    last_temp_read = 0;
}

/* CHECK TEMPERATURE*/
bool temperature_triggered() { 
    
    unsigned long now = millis();

    if (now - last_temp_read < TEMP_INTERVAL) // Only accept calls after interval has gone.
        return false;

    last_temp_read = now;
    
    float temperature = dht.readTemperature();
    Serial.println(temperature);
    if (isnan(temperature))
        return false;

    return temperature >= TEMP_THRESHOLD; // Returns True if triggered.
}