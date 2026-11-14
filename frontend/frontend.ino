/**
 * @file frontend.ino
 * @brief Main Arduino escape-room sketch.
 */
/*
 * ============================================================================
 *  PROJECT     : IoT Escape Room - Operation Breakout
 *  FILE        : frontend.ino
 *  PLATFORM    : Arduino Pro Mini (ATmega328P)
 *  VERSION     : 0.1.0
 *  AUTHORS     : Peter Boye Hansen
 *              : Boas Miedema
 *              : Stefano Giroletti
 *  LICENSE     : MIT
 * ============================================================================
 */


#include "game.h"

/**
 * @brief Initialize the Arduino sketch.
 */
void setup() {
    game_setup();
}

/**
 * @brief Run one iteration of the Arduino sketch.
 */
void loop() {
    game_task();
}