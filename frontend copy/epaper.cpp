#include <Arduino.h>
#include <SPI.h>
#include "epaper.h"
#include "src/epd1in54_V2/epd1in54_V2.h"
#include "src/epd1in54_V2/epdpaint.h"

#define COLORED   0
#define UNCOLORED 1

Epd epd;

unsigned char image[1024];
Paint paint(image, 0, 0);

static bool code_displayed = false;


// Reset display flag
void reset_show_code() {
  code_displayed = false;
}


// Initialize e-paper
void epaper_init() {
  Serial.begin(9600);

  epd.LDirInit();
  epd.Clear();

  paint.SetWidth(200);
  paint.SetHeight(24);
}


// Draw a single line of text
void draw_file_text(int y, String text, bool large = false) {
  paint.SetWidth(200);
  paint.SetHeight(large ? 24 : 16);
  paint.Clear(UNCOLORED);

  paint.DrawStringAt(
    4, 1,
    text.c_str(),
    large ? &Font16 : &Font12,
    COLORED
  );

  epd.SetFrameMemory(
    paint.GetImage(),
    0, y,
    paint.GetWidth(),
    paint.GetHeight()
  );
}


// Draw anonymous profile icon
void draw_profile_icon() {
  paint.SetWidth(64);
  paint.SetHeight(64);
  paint.Clear(UNCOLORED);

  // Frame
  paint.DrawRectangle(0, 0, 63, 63, COLORED);

  // Head
  paint.DrawCircle(32, 22, 11, COLORED);

  // Body
  paint.DrawLine(12, 57, 12, 49, COLORED);
  paint.DrawLine(12, 49, 21, 39, COLORED);
  paint.DrawLine(21, 39, 43, 39, COLORED);
  paint.DrawLine(43, 39, 52, 49, COLORED);
  paint.DrawLine(52, 49, 52, 57, COLORED);
  paint.DrawLine(12, 57, 52, 57, COLORED);

  epd.SetFrameMemory(
    paint.GetImage(),
    64, 43,
    64, 64
  );
}


// Display employee file
void show_employee_file(const Employee& employee) {
  if (code_displayed) return;
  
  epd.Clear();

  // Header
  draw_file_text(2, "ADMIN // RECORDS", true);
  draw_file_text(27, "FILE: " + employee.fileId);

  // Profile image
  draw_profile_icon();

  // Employee information
  draw_file_text(111, "NAME: [REDACTED]");
  draw_file_text(128, "DOG: " + employee.dogName);
  draw_file_text(145, "AGE: " + String(employee.age));
  draw_file_text(162, "DEPT: " + employee.department);

  // Footer
  draw_file_text(183, "STATUS: CLASSIFIED");

  epd.DisplayFrame();
  //epd.Sleep();

  code_displayed = true;
}


// Display puzzle code
void show_code(String code) {
  
  if (code_displayed) return;
  epd.Clear();
  
  Serial.println(code);

  paint.SetWidth(200);
  paint.SetHeight(24);
  paint.Clear(UNCOLORED);

  paint.DrawStringAt(10, 4, code.c_str(), &Font16, COLORED);

  epd.SetFrameMemory(
    paint.GetImage(),
    0, 10,
    paint.GetWidth(),
    paint.GetHeight()
  );

  epd.DisplayFrame();
  //epd.Sleep();

  code_displayed = true;
}