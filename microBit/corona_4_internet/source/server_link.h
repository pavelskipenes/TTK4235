#ifndef SERVER_LINK_H
#define SERVER_LINK_H
#include <stdint.h>

typedef struct {
    uint8_t button_a_active;
    uint8_t button_b_active;
    int16_t accelerometer_x;
    int16_t accelerometer_y;
    int16_t accelerometer_z;
} ServerLinkState;

void server_link_init(const char * student_id, const ServerLinkState * p_state);

#endif
