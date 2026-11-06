#include <Arduino.h>
#include "phase.h"
#include "pinout.h"

/* CONSTANTS */
const unsigned long PERIOD = 1000;
const unsigned long SECRET_OFFSET = 347;
const unsigned long SYNC_TIME = 3000;
const unsigned long SYNC_TOLERANCE = 30;

/* INITIALIZATION */
void phase_init() {
    pinMode(LED_REF_PIN, OUTPUT);
    pinMode(LED_ADJ_PIN, OUTPUT);
}

bool phase_update() {
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