/**
 * @file actuator.cpp
 * @brief Servo actuator positions and control.
 */
#include <Arduino.h>
#include <Servo.h>
#include "actuator.h"
#include "pinout.h"

/**
 * @brief Servo connected to the puzzle actuator.
 */
static Servo servo;
/** @brief Last requested servo position, or -1 before initialization. */
static int16_t current = -1;

/**
 * @brief Attach the servo to ACTUATOR_PIN and move it to zero degrees.
 */
void actuator_init() {
    servo.attach(ACTUATOR_PIN);
    actuator_set(DEGREE_0);
}

/**
 * @brief Move the servo to the requested puzzle position.
 * @param degree Target servo angle from actuator_degree.
 * @note Repeated requests for the current position are ignored.
 */
void actuator_set(actuator_degree degree) {
    if (degree == current) return;
    // Serial.println(degree);
    current = degree;
    servo.write(degree);
}