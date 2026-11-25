/**
 * @file epaper.cpp
 * @brief E-paper puzzle-code rendering.
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
 * @brief Guard preventing repeated code drawing.
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


// Display puzzle code
void show_code(const char* code) {

  if (code_displayed) return;
  epd.Clear();

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