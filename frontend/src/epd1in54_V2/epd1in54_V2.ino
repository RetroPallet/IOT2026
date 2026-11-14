/**
 * @file epd1in54_V2.ino
 * @brief Waveshare 1.54-inch V2 e-paper controller driver.
 */
#include <SPI.h>
#include "epd1in54_V2.h"
#include "imagedata.h"
#include "epdpaint.h"
#include <stdio.h>

/**
 * @brief E-paper controller instance.
 */
Epd epd;
/**
 * @brief Packed monochrome drawing buffer.
 */
unsigned char image[1024];
/**
 * @brief Drawing helper backed by image.
 */
Paint paint(image, 0, 0);

unsigned long time_start_ms;
unsigned long time_now_s;
/**
 * @brief E-paper foreground pixel value.
 */
#define COLORED     0
/**
 * @brief E-paper background pixel value.
 */
#define UNCOLORED   1

/**
 * @brief Initialize the Waveshare display demonstration.
 */
void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("e-Paper init and clear");
  epd.LDirInit();
  epd.Clear();

  paint.SetWidth(200);
  paint.SetHeight(24);

  Serial.println("e-Paper paint");
  paint.Clear(COLORED);
  paint.DrawStringAt(30, 4, "Hello world!", &Font16, UNCOLORED);
  epd.SetFrameMemory(paint.GetImage(), 0, 10, paint.GetWidth(), paint.GetHeight());

  paint.Clear(UNCOLORED);
  paint.DrawStringAt(30, 4, "e-Paper Demo", &Font16, COLORED);
  epd.SetFrameMemory(paint.GetImage(), 0, 30, paint.GetWidth(), paint.GetHeight());

  paint.SetWidth(64);
  paint.SetHeight(64);

  paint.Clear(UNCOLORED);
  paint.DrawRectangle(0, 0, 40, 50, COLORED);
  paint.DrawLine(0, 0, 40, 50, COLORED);
  paint.DrawLine(40, 0, 0, 50, COLORED);
  epd.SetFrameMemory(paint.GetImage(), 16, 60, paint.GetWidth(), paint.GetHeight());

  paint.Clear(UNCOLORED);
  paint.DrawCircle(32, 32, 30, COLORED);
  epd.SetFrameMemory(paint.GetImage(), 120, 60, paint.GetWidth(), paint.GetHeight());

  paint.Clear(UNCOLORED);
  paint.DrawFilledRectangle(0, 0, 40, 50, COLORED);
  epd.SetFrameMemory(paint.GetImage(), 16, 130, paint.GetWidth(), paint.GetHeight());

  paint.Clear(UNCOLORED);
  paint.DrawFilledCircle(32, 32, 30, COLORED);
  epd.SetFrameMemory(paint.GetImage(), 120, 130, paint.GetWidth(), paint.GetHeight());
  epd.DisplayFrame();
  delay(2000);

  Serial.println("e-Paper show pic");
  epd.HDirInit();
  // epd.Display(IMAGE_DATA);

  //Part display
  epd.HDirInit();
  epd.DisplayPartBaseImage(IMAGE_DATA);

  paint.SetWidth(50);
  paint.SetHeight(60);
  paint.Clear(UNCOLORED);

  char i = 0;
  char str[10][10] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"};
  for (i = 0; i < 10; i++) {
    paint.Clear(UNCOLORED);
    paint.DrawStringAt(10, 10, str[i], &Font24, COLORED);
    epd.SetFrameMemoryPartial(paint.GetImage(), 80, 70, paint.GetWidth(), paint.GetHeight());
    epd.DisplayPartFrame();
    delay(100);
  }

  Serial.println("e-Paper clear and goto sleep");
  epd.HDirInit();
  epd.Clear();
  epd.Sleep();
}

/**
 * @brief Run the display demonstration loop.
 */
void loop()
{

}
