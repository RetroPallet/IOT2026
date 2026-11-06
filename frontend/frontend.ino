/*
 * ============================================================================
 *  PROJECT     : IoT Escape Room - Operation Breakout
 *  FILE        : frontend.ino
 *  PLATFORM    : Arduino Pro Mini (ATmega328P)
 *  VERSION     : 0.1.0
 *  AUTHORS     : Peter Boye Hansen
 *  LICENSE     : MIT
 * ============================================================================
 */


#include "game.h"

void setup() {
    game_setup();
}

void loop() {
    game_task();
}