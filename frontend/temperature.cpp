#include <Arduino.h>
#include <DHT.h>
#include "temperature.h"
#include "pinout.h"

/* CONSTANTS */
#define DHT_TYPE DHT11

const float TEMP_THRESHOLD = 25.0;
const unsigned long TEMP_INTERVAL = 2000;

/* VARIABLES */
DHT dht(DHT_PIN, DHT_TYPE);

unsigned long last_temp_read = 0;
static bool temp_hot = false;   

/* INITIALIZATION */
void temperature_init() {
    dht.begin();
    last_temp_read = 0;
    temp_hot = false;
}

bool temperature_triggered() {
    unsigned long now = millis();

    if (now - last_temp_read < TEMP_INTERVAL)
        return temp_hot;

    last_temp_read = now;

    float temperature = dht.readTemperature();
    // Serial.println(temperature);

    if (isnan(temperature))
        return temp_hot;

    temp_hot = temperature >= TEMP_THRESHOLD;
    return temp_hot;
}