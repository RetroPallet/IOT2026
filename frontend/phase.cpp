/**
 * @file phase.cpp
 * @brief Potentiometer-controlled LED phase puzzle.
 */
#include <Arduino.h>
#include "phase.h"
#include "pinout.h"

/* CONSTANTS */
/**
 * @brief Blink period in milliseconds.
 */
const unsigned long PERIOD = 1000;
/**
 * @brief Hidden phase shift in milliseconds.
 */
const unsigned long SECRET_OFFSET = 347;
/**
 * @brief Required continuous synchronization duration in milliseconds.
 */
const unsigned long SYNC_TIME = 3000;
/**
 * @brief Allowed phase error in milliseconds around zero.
 */
const unsigned long SYNC_TOLERANCE = 30;

/* INITIALIZATION */
/**
 * @brief Configure the reference and adjustable phase LEDs as outputs.
 */
void phase_init() {
    pinMode(LED_REF_PIN, OUTPUT);
    pinMode(LED_ADJ_PIN, OUTPUT);
}

/**
 * @brief Update both blinking LEDs from the potentiometer-controlled phase offset.
 * @return True after the phase remains within SYNC_TOLERANCE for at least SYNC_TIME milliseconds.
 */
bool phase_update() { // returns true on synced.
    static unsigned long sync_started_at = 0;

    unsigned long now = millis();

    int pot_value = analogRead(POT_PIN);

    unsigned long phase = map(pot_value, 0, 1023, 0, PERIOD - 1);
    phase = (phase + SECRET_OFFSET) % PERIOD;

    unsigned long ref_time = now % PERIOD;
    unsigned long adj_time = (now + phase) % PERIOD;

    digitalWrite(LED_REF_PIN, ref_time < PERIOD / 2);
    digitalWrite(LED_ADJ_PIN, adj_time < PERIOD / 2);

    bool synchronized = (phase <= SYNC_TOLERANCE ||
                         phase >= PERIOD - SYNC_TOLERANCE);

    if (!synchronized) {
        sync_started_at = 0;
        return false;
    }

    if (sync_started_at == 0) {
        sync_started_at = now;
    }

    return now - sync_started_at >= SYNC_TIME;
}