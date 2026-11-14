/**
 * @file temperature.h
 * @brief DHT11 temperature threshold puzzle.
 */
#ifndef TEMPERATURE_H
#define TEMPERATURE_H

/**
 * @brief Initialize the DHT sensor and reset the cached temperature state.
 */
void temperature_init();
/**
 * @brief Update the cached temperature threshold state at TEMP_INTERVAL intervals.
 * @return True if the last valid temperature is at least TEMP_THRESHOLD.
 * @note Invalid sensor readings retain the previous state.
 */
bool temperature_triggered();
#endif