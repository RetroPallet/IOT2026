/**
 * @file pinout.h
 * @brief Arduino peripheral pin assignments.
 */
#ifndef PINOUT_H
#define PINOUT_H

#include <Arduino.h>

/* ============================================================
 * PINOUT (Arduino Uno / Nano)
 * ============================================================
 *  D0  RX serial (reserved)        D8  EPD RST
 *  D1  TX serial (reserved)        D9  EPD DC
 *  D2  Button                      D10 EPD CS
 *  D3  LED RGB - BLUE (PWM)        D11 EPD DIN/MOSI
 *  D4  DHT11                       D12 LoRa RN2483 TX -> Arduino RX
 *  D5  LED RGB - RED (PWM)         D13 EPD CLK/SCK
 *  D6  LED RGB - GREEN (PWM)       A0  Potentiometer
 *  D7  EPD BUSY                    A1  Reference LED
 *                                  A2  Tunable LED
 *                                  A3  LoRa RN2483 RX <- Arduino TX
 *                                  A4  LoRa RN2483 RESET
 *                                  A5  free
 * ============================================================ */

/**
 * @brief Arduino pin assignment for button.
 */
#define BUTTON_PIN      2
/**
 * @brief Arduino pin assignment for the led blue
 */
#define LED_BLUE_PIN    3
/**
 * @brief Arduino pin assignment for dht.
 */
#define DHT_PIN         4
/**
 * @brief Arduino pin assignment for led red.
 */
#define LED_RED_PIN     5
/**
 * @brief Arduino pin assignment for led green.
 */
#define LED_GREEN_PIN   6

// D7-D11, D13: e-paper (defined in epdif.h)
/**
 * @brief Arduino pin assignment for lora rx.
 */
#define LORA_RX_PIN     12   // Arduino RX <- RN2483 TX
/**
 * @brief Arduino pin assignment for pot.
 */
#define POT_PIN         A0
/**
 * @brief Arduino pin assignment for led ref.
 */
#define LED_REF_PIN     A1
/**
 * @brief Arduino pin assignment for led adj.
 */
#define LED_ADJ_PIN     A2
/**
 * @brief Arduino pin assignment for lora tx.
 */
#define LORA_TX_PIN     A3   // Arduino TX -> RN2483 RX (3.3V level!)
/**
 * @brief Arduino pin assignment for lora reset.
 */
#define LORA_RESET_PIN  A4   // RN2483 RESET (active low)

#endif