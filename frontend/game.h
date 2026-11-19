/**
 * @file game.h
 * @brief Escape-room game states and puzzle coordination.
 */
#ifndef GAME_H
#define GAME_H

#include <Arduino.h>

/**
 * @brief Device game states, including the temporary backend-wait state.
 */
enum game_state {
    WAIT_INITIALIZATION, /**< Wait for game initialization. */
    PHASE_PUZZLE, /**< Run the LED phase and temperature puzzle. */
    SERIAL_PUZZLE, /**< Run the serial password puzzle. */
    BACKEND_PUZZLE, /**< Wait while the player solves the server puzzle. */
    WAIT_BACKEND, /**< Request the next state from the backend. */
    COMPLETED /**< Game completed. */
};

/**
 * @brief Identifiers for the device puzzles.
 */
enum puzzle_id {
    TEMP_PUZZLE_ID,
    PHASE_PUZZLE_ID,
    SERIAL_PUZZLE_ID
};

/**
 * @brief Current device game state.
 */
extern game_state state;

/**
 * @brief Initialize serial communication, peripherals and the initial game-state outputs.
 */
void game_setup();
/**
 * @brief Run one game-state iteration, update puzzles and process requested backend transitions.
 * @note Backend communication and display updates may block.
 */
void game_task();
/**
 * @brief Change the game state and update the status LED and actuator.
 * @param new_state Game state to enter.
 * @note WAIT_BACKEND preserves the actuator position; an unchanged state has no effect.
 */
void set_state(game_state new_state);

#endif