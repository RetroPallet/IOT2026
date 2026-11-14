/**
 * @file epdif.h
 * @brief Arduino GPIO and SPI interface for the e-paper display.
 */
/**
 *  File   :   epdif.h
 *  @brief      :   Header file of epdif.cpp providing EPD interface functions
 *                  Users have to implement all the functions in epdif.cpp
 *  @author     :   Yehui from Waveshare
 *
 *  Copyright (C) Waveshare     August 10 2017
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documnetation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to  whom the Software is
 * furished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef EPDIF_H
#define EPDIF_H

#include <Arduino.h>

// Pin definition
/**
 * @brief Arduino pin assignment for rst.
 */
#define RST_PIN         8
/**
 * @brief Arduino pin assignment for dc.
 */
#define DC_PIN          9
/**
 * @brief Arduino pin assignment for cs.
 */
#define CS_PIN          10
/**
 * @brief Arduino pin assignment for busy.
 */
#define BUSY_PIN        7
/**
 * @brief Arduino pin assignment for pwr.
 */
#define PWR_PIN         6

/**
 * @brief Arduino GPIO and SPI adaptation layer.
 */
class EpdIf {
public:
    /**
     * @brief Initialize the EpdIf object.
     */
    EpdIf(void);
    /**
     * @brief Release the EpdIf object.
     */
    ~EpdIf(void);

    /**
     * @brief Configure display GPIO and start SPI at 2 MHz in mode 0.
     * @return Zero after configuring the interface.
     */
    static int  IfInit(void);
    /**
     * @brief Write a logic level to an Arduino pin.
     * @param pin Arduino pin number.
     * @param value Logic level to write.
     */
    static void DigitalWrite(int pin, int value); 
    /**
     * @brief Read the logic level of an Arduino pin.
     * @param pin Arduino pin number.
     * @return The pin level, HIGH or LOW.
     */
    static int  DigitalRead(int pin);
    /**
     * @brief Pause execution for the requested number of milliseconds.
     * @param delaytime Delay in milliseconds.
     */
    static void DelayMs(unsigned int delaytime);
    /**
     * @brief Transmit one SPI byte while asserting the display chip-select signal.
     * @param data Data byte to transmit.
     */
    static void SpiTransfer(unsigned char data);
};

#endif
