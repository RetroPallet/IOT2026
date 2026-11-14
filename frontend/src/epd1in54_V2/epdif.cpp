/**
 * @file epdif.cpp
 * @brief Arduino GPIO and SPI interface for the e-paper display.
 */
/**
 *  File   :   epdif.cpp
 *  @brief      :   Implements EPD interface functions
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

#include "epdif.h"
#include <SPI.h>

/* Initialize the EpdIf object. */
EpdIf::EpdIf() {
};

/* Release the EpdIf object. */
EpdIf::~EpdIf() {
};

/* Write a logic level to an Arduino pin. */
void EpdIf::DigitalWrite(int pin, int value) {
    digitalWrite(pin, value);
}

/* Read the logic level of an Arduino pin. */
int EpdIf::DigitalRead(int pin) {
    return digitalRead(pin);
}

/* Pause execution for the requested number of milliseconds. */
void EpdIf::DelayMs(unsigned int delaytime) {
    delay(delaytime);
}

/* Transmit one SPI byte while asserting the display chip-select signal. */
void EpdIf::SpiTransfer(unsigned char data) {
    digitalWrite(CS_PIN, LOW);
    SPI.transfer(data);
    digitalWrite(CS_PIN, HIGH);
}

/* Configure display GPIO and start SPI at 2 MHz in mode 0. */
int EpdIf::IfInit(void) {
    pinMode(CS_PIN, OUTPUT);
    pinMode(RST_PIN, OUTPUT);
    pinMode(DC_PIN, OUTPUT);
    pinMode(BUSY_PIN, INPUT); 

    pinMode(PWR_PIN, OUTPUT);
    DigitalWrite(PWR_PIN, 1);

    SPI.begin();
    SPI.beginTransaction(SPISettings(2000000, MSBFIRST, SPI_MODE0));
    return 0;
}

