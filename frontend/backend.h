#ifndef BACKEND_H
#define BACKEND_H

enum state_codes {
    WAIT_INITIALIZATION_CODE = 1,
    PHASE_PUZZLE_CODE = 2,
    SERIAL_PUZZLE_CODE = 3,
    BACKEND_PUZZLE_CODE = 4,
    COMPLETED_CODE = 5,
    NO_DOWNLINK_CODE = 0
};

void backend_init();
state_codes get_be_next_state_code();

#endif