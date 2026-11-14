/**
 * @file backend.h
 * @brief LoRaWAN communication and game-state downlinks.
 */
#ifndef BACKEND_H
#define BACKEND_H

/**
 * @brief One-byte backend state codes; zero means no usable downlink.
 */
enum state_codes {
    WAIT_INITIALIZATION_CODE = 1, /**< Downlink code for wait initialization. */
    PHASE_PUZZLE_CODE = 2, /**< Downlink code for phase puzzle. */
    SERIAL_PUZZLE_CODE = 3, /**< Downlink code for serial puzzle. */
    BACKEND_PUZZLE_CODE = 4, /**< Downlink code for backend puzzle. */
    COMPLETED_CODE = 5, /**< Downlink code for completed. */
    NO_DOWNLINK_CODE = 0 /**< No valid backend state received. */
};

/**
 * @brief Reset the RN2483 and initialize the LoRaWAN connection.
 */
void backend_init();
/**
 * @brief Send a one-byte uplink and decode the next game state from a downlink.
 * @return A valid state code, or NO_DOWNLINK_CODE on failure or absent/invalid downlink.
 */
state_codes get_be_next_state_code();

#endif