#include <Arduino.h>
#include <Servo.h>
#include "actuator.h"
#include "pinout.h"

static Servo servo;
static int16_t current = -1;

void actuator_init() {
    servo.attach(ACTUATOR_PIN);
    actuator_set(DEGREE_0);
}

void actuator_set(actuator_degree degree) {
    if (degree == current) return;
    // Serial.println(degree);
    current = degree;
    servo.write(degree);
}