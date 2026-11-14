/**
 * @file phase.h
 * @brief Potentiometer-controlled LED phase puzzle.
 */
#ifndef PHASE_H
#define PHASE_H

/**
 * @brief Configure the reference and adjustable phase LEDs as outputs.
 */
void phase_init();
/**
 * @brief Update both blinking LEDs from the potentiometer-controlled phase offset.
 * @return True after the phase remains within SYNC_TOLERANCE for at least SYNC_TIME milliseconds.
 */
bool phase_update();

#endif