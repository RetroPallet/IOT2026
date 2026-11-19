/**
 * @file epaper.h
 * @brief E-paper puzzle-code and employee-record rendering.
 */
#ifndef EPAPER_H
#define EPAPER_H

/**
 * @brief Employee data shown by the serial puzzle.
 */
struct Employee {
  /**
   * @brief Employee file identifier.
   */
  String fileId;
  /**
   * @brief Name of the employee's dog.
   */
  String dogName;
  /**
   * @brief Employee age in years.
   */
  int age;
  /**
   * @brief Employee department.
   */
  String department;
  /**
   * @brief Additional employee clue.
   */
  String hint;
};

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
 * @fn void draw_file_text(int y, String text, bool large)
 * @brief Draw a line of text into the employee-file frame memory.
 * @param y Vertical pixel coordinate.
 * @param text Text to draw.
 * @param large Use Font16 when true, otherwise Font12.
 * @note The caller refreshes the screen after composing the complete frame.
 */
void draw_file_text(int y, String text, bool large = false);
/**
 * @fn void draw_profile_icon()
 * @brief Draw an anonymous profile icon into the employee-file frame memory.
 */
void draw_profile_icon();
/**
 * @fn void show_employee_file(const Employee& employee)
 * @brief Compose and display the employee record with a profile icon and redacted name.
 * @param employee Employee record to render.
 * @note The display guard prevents repeated drawing until reset_show_code() is called.
 */
void show_employee_file(const Employee& employee);
/**
 * @brief Allow the next show_code() call to draw a code again.
 * @note This resets the display guard without clearing the screen or changing the visibility timer.
 */
void reset_show_code();

#endif