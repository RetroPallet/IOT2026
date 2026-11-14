/**
 * @file epd1in54_V2.h
 * @brief Waveshare 1.54-inch V2 e-paper controller driver.
 */
/*****************************************************************************
* | File      	:   epd1in54_V2.h
* | Author      :   Waveshare team
* | Function    :   1.54inch e-paper V2
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2019-06-24
* | Info        :
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documnetation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to  whom the Software is
# furished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
#
******************************************************************************/

#ifndef epd1in54_V2_H
#define epd1in54_V2_H

#include "epdif.h"

// Display resolution
/**
 * @brief Display dimension in pixels.
 */
#define EPD_WIDTH       200
/**
 * @brief Display dimension in pixels.
 */
#define EPD_HEIGHT      200

/**
 * @brief Controller interface for the 200 by 200 pixel e-paper display.
 */
class Epd : EpdIf
{
public:
	/**
	 * @brief Image or display width in pixels.
	 */
	unsigned long width;
	/**
	 * @brief Image or display height in pixels.
	 */
	unsigned long height;

	/**
	 * @brief Initialize the Epd object.
	 */
	Epd();
	/**
	 * @brief Release the Epd object.
	 */
	~Epd();
	// int  Init(void);
	/**
	 * @brief Initialize the display controller for the low-direction scanning mode.
	 * @return Zero on success, or -1 if interface initialization fails.
	 */
	int LDirInit(void);
	/**
	 * @brief Initialize the display controller for the high-direction scanning mode.
	 * @return Zero on success, or -1 if interface initialization fails.
	 */
	int HDirInit(void);
	/**
	 * @brief Transmit a controller command over SPI.
	 * @param command Controller command byte.
	 */
	void SendCommand(unsigned char command);
	/**
	 * @brief Transmit one data byte over SPI.
	 * @param data Data byte to transmit.
	 */
	void SendData(unsigned char data);
	/**
	 * @brief Wait until the e-paper controller releases its busy signal.
	 */
	void WaitUntilIdle(void);
	/**
	 * @brief Pulse the display reset pin.
	 */
	void Reset(void);
	/**
	 * @brief Clear the display to white.
	 */
	void Clear(void);
	/**
	 * @brief Write a full-screen bitmap and perform a full refresh.
	 * @param frame_buffer Packed one-bit full-screen bitmap.
	 */
	void Display(const unsigned char* frame_buffer);
	/**
	 * @brief Write a bitmap as the base image for subsequent partial refreshes.
	 * @param frame_buffer Packed one-bit full-screen bitmap.
	 */
	void DisplayPartBaseImage(const unsigned char* frame_buffer);
	/**
	 * @brief Initialize the partial-refresh base image to white.
	 */
	void DisplayPartBaseWhiteImage(void);
	/**
	 * @brief Write a full-screen bitmap using the partial-refresh waveform.
	 * @param frame_buffer Packed one-bit full-screen bitmap.
	 */
	void DisplayPart(const unsigned char* frame_buffer);
	/**
	 * @brief Copy a bitmap region into controller frame memory.
	 * @param image_buffer Packed one-bit source bitmap.
	 * @param x Horizontal pixel coordinate.
	 * @param y Vertical pixel coordinate.
	 * @param image_width Source bitmap width in pixels.
	 * @param image_height Source bitmap height in pixels.
	 * @note Horizontal coordinates and width are rounded down to multiples of eight; call DisplayFrame() to refresh.
	 */
	void SetFrameMemory(
	        const unsigned char* image_buffer,
	        int x,
	        int y,
	        int image_width,
	        int image_height
	);
	/**
	 * @brief Copy a bitmap region into controller memory using partial-update settings.
	 * @param image_buffer Packed one-bit source bitmap.
	 * @param x Horizontal pixel coordinate.
	 * @param y Vertical pixel coordinate.
	 * @param image_width Source bitmap width in pixels.
	 * @param image_height Source bitmap height in pixels.
	 * @note Horizontal coordinates and width are aligned to eight pixels; call DisplayPartFrame() to refresh.
	 */
	void SetFrameMemoryPartial(
	        const unsigned char* image_buffer,
	        int x,
	        int y,
	        int image_width,
	        int image_height
	);
	/**
	 * @brief Trigger a full refresh from controller frame memory and wait for completion.
	 */
	void DisplayFrame(void);
	/**
	 * @brief Trigger a partial refresh from controller frame memory and wait for completion.
	 */
	void DisplayPartFrame(void);

	/**
	 * @brief Put the display controller into deep sleep.
	 * @note Reinitialize the controller before sending further display commands.
	 */
	void Sleep(void);
private:
	/**
	 * @brief Controller reset GPIO.
	 */
	unsigned int reset_pin;
	/**
	 * @brief Data/command selection GPIO.
	 */
	unsigned int dc_pin;
	/**
	 * @brief SPI chip-select GPIO.
	 */
	unsigned int cs_pin;
	/**
	 * @brief Controller busy input GPIO.
	 */
	unsigned int busy_pin;

	/**
	 * @brief Write the first 153 bytes of the waveform lookup table.
	 * @param lut Waveform lookup table; SetLut requires at least 159 bytes.
	 */
	void Lut(unsigned char* lut);
	/**
	 * @brief Load a waveform table and its controller voltage settings.
	 * @param lut Waveform lookup table; SetLut requires at least 159 bytes.
	 */
	void SetLut(unsigned char* lut);
	/**
	 * @brief Set the rectangular controller RAM address window.
	 * @param x_start Left edge of the RAM window in pixels.
	 * @param y_start Top edge of the RAM window in pixels.
	 * @param x_end Inclusive right edge in pixels.
	 * @param y_end Inclusive bottom edge in pixels.
	 */
	void SetMemoryArea(int x_start, int y_start, int x_end, int y_end);
	/**
	 * @brief Set the controller RAM write cursor.
	 * @param x Horizontal pixel coordinate.
	 * @param y Vertical pixel coordinate.
	 */
	void SetMemoryPointer(int x, int y);
};

#endif /* EPD1IN54B_H */

/* END OF FILE */
