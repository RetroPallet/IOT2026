/**
 * @file epaper.cpp
 * @brief E-paper puzzle-code and employee-record rendering.
 */
#include <Arduino.h>
#include <SPI.h>
#include "epaper.h"
#include "src/epd1in54_V2/epd1in54_V2.h"
#include "src/epd1in54_V2/epdpaint.h"

/**
 * @brief E-paper foreground pixel value.
 */
#define COLORED   0
/**
 * @brief E-paper background pixel value.
 */
#define UNCOLORED 1

/**
 * @brief Code visibility timeout in milliseconds.
 */
#define CODE_DISPLAY_MS 10000UL

/**
 * @brief E-paper controller instance.
 */
Epd epd;

/**
 * @brief Packed monochrome drawing buffer.
 */
unsigned char image[600];
/**
 * @brief Drawing helper backed by image.
 */
Paint paint(image, 0, 0);

/**
 * @brief Guard preventing repeated code or employee-record drawing.
 */
static bool code_displayed = false;
/**
 * @brief Whether the code display timeout is active.
 */
static bool code_visible = false;        
/**
 * @brief Code display start time from millis().
 */
static unsigned long code_shown_at = 0;  


// Reset display flag
/* Allow the next show_code() call to draw a code again. */
void reset_show_code() {
  code_displayed = false;
}


// Initialize e-paper
/* Initialize and clear the e-paper display and configure a 200 by 24 pixel drawing area. */
void epaper_init() {
  epd.LDirInit();
  epd.Clear();

  paint.SetWidth(200);
  paint.SetHeight(24);
}


/* Clear the displayed code after CODE_DISPLAY_MS and allow another code to be shown. */
void epaper_task() {
  if (!code_visible) return;

  if (millis() - code_shown_at >= CODE_DISPLAY_MS) {
    epd.Clear();
    code_visible = false;
    reset_show_code();
  }
}


/*
 * @brief Draw a line of text into the employee-file frame memory.
 * @param y Vertical pixel coordinate.
 * @param text Text to draw.
 * @param large Use Font16 when true, otherwise Font12.
 * @note The caller refreshes the screen after composing the complete frame.
 */
// void draw_file_text(int y, String text, bool large=false) {
//   paint.SetWidth(200);
//   paint.SetHeight(large ? 24 : 16);
//   paint.Clear(UNCOLORED);

//   paint.DrawStringAt(
//     4, 1,
//     text.c_str(),
//     large ? &Font16 : &Font12,
//     COLORED
//   );

//   epd.SetFrameMemory(
//     paint.GetImage(),
//     0, y,
//     paint.GetWidth(),
//     paint.GetHeight()
//   );
// }


// Draw anonymous profile icon
/*
 * @brief Draw an anonymous profile icon into the employee-file frame memory.
 */
// void draw_profile_icon() {
//   paint.SetWidth(64);
//   paint.SetHeight(64);
//   paint.Clear(UNCOLORED);

//   // Frame
//   paint.DrawRectangle(0, 0, 63, 63, COLORED);

//   // Head
//   paint.DrawCircle(32, 22, 11, COLORED);

//   // Body
//   paint.DrawLine(12, 57, 12, 49, COLORED);
//   paint.DrawLine(12, 49, 21, 39, COLORED);
//   paint.DrawLine(21, 39, 43, 39, COLORED);
//   paint.DrawLine(43, 39, 52, 49, COLORED);
//   paint.DrawLine(52, 49, 52, 57, COLORED);
//   paint.DrawLine(12, 57, 52, 57, COLORED);

//   epd.SetFrameMemory(
//     paint.GetImage(),
//     64, 43,
//     64, 64
//   );
// }


// Display employee file
/*
 * @brief Compose and display the employee record with a profile icon and redacted name.
 * @param employee Employee record to render.
 * @note The display guard prevents repeated drawing until reset_show_code() is called.
 */
// void show_employee_file(const Employee& employee) {
//   if (code_displayed) return;

//   epd.Clear();

//   // Header
//   draw_file_text(2, "ADMIN // RECORDS", true);
//   draw_file_text(27, "FILE: " + employee.fileId);

//   // Profile image
//   draw_profile_icon();

//   // Employee information
//   draw_file_text(111, "NAME: [REDACTED]");
//   draw_file_text(128, "DOG: " + employee.dogName);
//   draw_file_text(145, "AGE: " + String(employee.age));
//   draw_file_text(162, "DEPT: " + employee.department);

//   // Footer
//   draw_file_text(183, "STATUS: CLASSIFIED");

//   epd.DisplayFrame();
//   //epd.Sleep();

//   code_displayed = true;
// }

// Display puzzle code
void show_code(const char* code) {

  if (code_displayed) return;
  epd.Clear();

  // Serial.println(code);

  paint.SetWidth(200);
  paint.SetHeight(24);
  paint.Clear(UNCOLORED);

  paint.DrawStringAt(10, 4, code, &Font16, COLORED);

  epd.SetFrameMemory(
    paint.GetImage(),
    0, 10,
    paint.GetWidth(),
    paint.GetHeight()
  );

  epd.DisplayFrame();

  code_displayed = true;
  code_visible   = true;       
  code_shown_at  = millis();
}