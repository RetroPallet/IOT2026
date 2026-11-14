#ifndef ACTUATOR_H
#define ACTUATOR_H

#include <stdint.h>

enum actuator_degree : uint8_t {
    DEGREE_0  = 0,
    DEGREE_22 = 22,
    DEGREE_44 = 44,
    DEGREE_66 = 66,
    DEGREE_90 = 90
};

void actuator_init();
void actuator_set(actuator_degree degree);

#endif