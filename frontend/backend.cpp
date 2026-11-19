// backend.cpp
#include <Arduino.h>
#include <rn2xx3.h>
#include <SoftwareSerial.h>
#include "pinout.h"
#include "backend.h"

static SoftwareSerial lora_serial(LORA_RX_PIN, LORA_TX_PIN);
static rn2xx3 lora(lora_serial);

static const char *APP_EUI = "xxx";
static const char *APP_KEY = "xxx";

void backend_init() {
    Serial.println(F("1 reset"));
    pinMode(LORA_RESET_PIN, OUTPUT);
    digitalWrite(LORA_RESET_PIN, LOW);
    delay(500);
    digitalWrite(LORA_RESET_PIN, HIGH);
    delay(100);

    lora_serial.begin(9600);
    lora_serial.flush();
    lora.autobaud();
    Serial.println(F("2 autobaud ok"));

    if (lora.hweui().length() != 16)
        return;

    Serial.println(F("3 hweui ok"));
    // return;

    bool join_result = lora.initOTAA(APP_EUI, APP_KEY);
    Serial.println(F("4 join"));

    while(!join_result)
    {
        Serial.println(F("Unable to join. Are your keys correct, and do you have TTN coverage?"));
        return;
        delay(60000); //delay a minute before retry
        join_result = lora.init();
    }
    Serial.println(F("Successfully joined TTN"));
}

static int hex_nibble(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    return -1;
}

// 1 = send the uplink without waiting for the radio, always return NO_DOWNLINK.
// 0 = real behaviour (blocking txBytes + downlink parsing).
#define SIMULATE_NO_DOWNLINK 1

state_codes get_be_next_state_code() {
#if SIMULATE_NO_DOWNLINK
    // Drop any pending reply from the previous uplink (e.g. "not_joined").
    while (lora_serial.available())
        lora_serial.read();

    // Same command txBytes() would send, but fire-and-forget: no wait,
    // no retries, no automatic re-join inside the library.
    lora_serial.print(F("mac tx uncnf 1 01\r\n"));
    Serial.println(F("uplink sent"));
    return NO_DOWNLINK_CODE;
#else
    const byte uplink[1] = { 0x01 };

    switch (lora.txBytes(uplink, sizeof(uplink))) {

        case TX_FAIL:
        case TX_SUCCESS:
            return NO_DOWNLINK_CODE;

        case TX_WITH_RX: {
            String received = lora.getRx();  // Hex payload
            received.trim();

            if (received.length() == 2) {
                int hi = hex_nibble(received[0]);
                int lo = hex_nibble(received[1]);

                if (hi >= 0 && lo >= 0) {
                    int value = hi * 16 + lo;
                    if (value >= WAIT_INITIALIZATION_CODE && value <= COMPLETED_CODE) {
                        return (state_codes)value;
                    }
                }
            }

            Serial.print(F("Invalid downlink: "));
            Serial.println(received);
            return NO_DOWNLINK_CODE;
        }

        default:
            Serial.println(F("Unknown response from TX function"));
            return NO_DOWNLINK_CODE;
    }
#endif
}
