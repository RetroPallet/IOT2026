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

/* ============================================================
 * Raw commands towards l'RN2483
 * ============================================================ */

static String lora_read_line(unsigned long timeout_ms) {
    lora_serial.setTimeout(timeout_ms);
    String r = lora_serial.readStringUntil('\n');
    r.trim();
    return r;
}

static bool lora_cmd(const __FlashStringHelper *cmd, const char *arg = nullptr) {
    while (lora_serial.available()) lora_serial.read();
    lora_serial.print(cmd);
    if (arg) lora_serial.print(arg);
    lora_serial.print(F("\r\n"));

    String r = lora_read_line(3000);
    if (r != "ok") {
        Serial.print(F("ERR ")); Serial.print(cmd);
        Serial.print(F(" -> ")); Serial.println(r);
        return false;
    }
    return true;
}

static bool lora_config(const String &eui) {
    return lora_cmd(F("mac reset 868"))
        && lora_cmd(F("mac set deveui "), eui.c_str())
        && lora_cmd(F("mac set appeui "), APP_EUI)
        && lora_cmd(F("mac set appkey "), APP_KEY)
        && lora_cmd(F("mac set rx2 3 869525000"))
        && lora_cmd(F("mac set adr off"))
        && lora_cmd(F("mac set dr 0"));
}

static bool lora_join() {
    if (!lora_cmd(F("mac join otaa"))) return false;

    String r = lora_read_line(20000);
    Serial.print(F("join: ")); Serial.println(r);
    return r.startsWith("accepted");
}

/* ============================================================
 * Init
 * ============================================================ */

void backend_init() {
    Serial.println(F("1 reset"));
    pinMode(LORA_RESET_PIN, OUTPUT);
    digitalWrite(LORA_RESET_PIN, LOW);
    delay(500);
    digitalWrite(LORA_RESET_PIN, HIGH);
    delay(2000);

    lora_serial.begin(9600);
    lora.autobaud();
    Serial.println(F("2 autobaud"));

    String eui = lora.hweui();
    Serial.print(F("hweui: ")); Serial.println(eui);
    if (eui.length() != 16) {
        Serial.println(F("Error establishing communication"));
        return;
    }

    if (!lora_config(eui)) {
        Serial.println(F("Config LoRa failed"));
        return;
    }

    Serial.println(F("Joining (SF12)..."));
    while (!lora_join()) {
        delay(15000);
    }
    Serial.println(F("Successfully joined TTN"));
}

/* ============================================================
 * Uplink + downlink
 * ============================================================ */

static int hex_nibble(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    return -1;
}

static state_codes parse_downlink(const String &hex) {
    if (hex.length() == 2) {
        int hi = hex_nibble(hex[0]);
        int lo = hex_nibble(hex[1]);
        if (hi >= 0 && lo >= 0) {
            int value = hi * 16 + lo;
            if (value >= WAIT_INITIALIZATION_CODE && value <= COMPLETED_CODE)
                return (state_codes)value;
        }
    }
    Serial.print(F("Invalid downlink: "));
    Serial.println(hex);
    return NO_DOWNLINK_CODE;
}

state_codes get_be_next_state_code() {
    if (!lora_cmd(F("mac tx uncnf 1 01")))
        return NO_DOWNLINK_CODE;

    String r = lora_read_line(30000);

    if (r.startsWith("mac_rx ")) {
        int sp = r.indexOf(' ', 7);
        return parse_downlink(sp > 0 ? r.substring(sp + 1) : String());
    }

    if (r != "mac_tx_ok") {
        Serial.print(F("tx: ")); Serial.println(r);
    }
    return NO_DOWNLINK_CODE;
}