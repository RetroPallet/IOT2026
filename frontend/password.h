/**
 * @file password.h
 * @brief Serial password puzzle.
 */
#ifndef PASSWORD_H
#define PASSWORD_H

/**
 * @brief Consume available serial input and check newline-terminated password attempts.
 * @return True if at least one password processed by this call is correct.
 * @note Carriage returns are ignored and input is limited to INPUT_SIZE - 1 characters.
 */
bool password_update();

#endif