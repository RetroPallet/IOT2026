/**
 * @file actuator.h
 * @brief Servo actuator positions and control.
 */
#ifndef ACTUATOR_H
#define ACTUATOR_H

#include <stdint.h>

/**
 * @brief Supported servo positions in degrees.
 */
enum actuator_degree : uint8_t {
    DEGREE_0  = 0, /**< Servo position of 0 degrees. */
    DEGREE_22 = 22, /**< Servo position of 22 degrees. */
    DEGREE_44 = 44, /**< Servo position of 44 degrees. */
    DEGREE_66 = 66, /**< Servo position of 66 degrees. */
    DEGREE_90 = 90 /**< Servo position of 90 degrees. */
};

/**
 * @brief Attach the servo to ACTUATOR_PIN and move it to zero degrees.
 */
void actuator_init();
/**
 * @brief Move the servo to the requested puzzle position.
 * @param degree Target servo angle from actuator_degree.
 * @note Repeated requests for the current position are ignored.
 */
void actuator_set(actuator_degree degree);

#endif