/**
 * @file epaper.h
 * @brief E-paper puzzle-code rendering.
 */
#ifndef EPAPER_H
#define EPAPER_H

/**
 * @brief Initialize and clear the e-paper display and configure a 200 by 24 pixel drawing area.
 */
void epaper_init();
/**
 * @brief Clear the displayed code after CODE_DISPLAY_MS and allow another code to be shown.
 * @note Call regularly from the game loop; this function does not wait for the timeout.
 */
void epaper_task();   
/**
 * @brief Render a puzzle code and start its display timeout.
 * @param code Puzzle code to display.
 * @note Calls are ignored while the display guard is set.
 */
void show_code(const char* code);
/**
 * @brief Allow the next show_code() call to draw a code again.
 * @note This resets the display guard without clearing the screen or changing the visibility timer.
 */
void reset_show_code();

#endif