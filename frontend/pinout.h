#ifndef PINOUT_H
#define PINOUT_H

#include <Arduino.h>

/* ============================================================
 * PINOUT (Arduino Uno / Nano)
 * ============================================================
 *  D0  RX serial (reserved)        D8  EPD RST
 *  D1  TX serial (reserved)        D9  EPD DC
 *  D2  Button                      D10 EPD CS
 *  D3  LED RGB - BLUE (reserved)   D11 EPD DIN/MOSI
 *  D4  DHT11                       D12 LoRa RN2483 TX -> Arduino RX
 *  D5  LED RGB - RED (PWM)         D13 EPD CLK/SCK
 *  D6  LED RGB - GREEN (PWM)       A0  Potentiometer
 *  D7  EPD BUSY                    A1  Reference LED
 *                                  A2  Tunable LED
 *                                  A3  LoRa RN2483 RX <- Arduino TX
 *                                  A4  LoRa RN2483 RESET
 *                                  A5  free
 * ============================================================ */

#define BUTTON_PIN      2
#define LED_BLUE_PIN 3  
#define DHT_PIN         4
#define LED_RED_PIN     5
#define LED_GREEN_PIN   6

// D7-D11, D13: e-paper (defined in epdif.h)
#define LORA_RX_PIN     12   // Arduino RX <- RN2483 TX
#define POT_PIN         A0
#define LED_REF_PIN     A1
#define LED_ADJ_PIN     A2
#define LORA_TX_PIN     A3   // Arduino TX -> RN2483 RX (3.3V level!)
#define LORA_RESET_PIN  A4   // RN2483 RESET (active low)

#endif