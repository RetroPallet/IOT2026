/**
 * @file temperature.cpp
 * @brief DHT11 temperature threshold puzzle.
 */
#include <Arduino.h>
#include <DHT.h>
#include "temperature.h"
#include "pinout.h"

/* CONSTANTS */
/**
 * @brief DHT sensor model.
 */
#define DHT_TYPE DHT11

/**
 * @brief Temperature threshold in degrees Celsius.
 */
const float TEMP_THRESHOLD = 25.0;
/**
 * @brief Minimum sensor-read interval in milliseconds.
 */
const unsigned long TEMP_INTERVAL = 2000;

/* VARIABLES */
/**
 * @brief DHT temperature sensor interface.
 */
DHT dht(DHT_PIN, DHT_TYPE);

/**
 * @brief Time of the last temperature read attempt in milliseconds.
 */
unsigned long last_temp_read = 0;
/**
 * @brief Cached threshold result from the last valid sensor read.
 */
static bool temp_hot = false;   

/* INITIALIZATION */
/**
 * @brief Initialize the DHT sensor and reset the cached temperature state.
 */
void temperature_init() {
    dht.begin();
    last_temp_read = 0;
    temp_hot = false;
}

/**
 * @brief Update the cached temperature threshold state at TEMP_INTERVAL intervals.
 * @return True if the last valid temperature is at least TEMP_THRESHOLD.
 * @note Invalid sensor readings retain the previous state.
 */
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