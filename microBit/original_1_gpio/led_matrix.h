#ifndef _LED_MATRIX_H_
#define _LED_MATRIX_H_

/* Pins 13, 14, 15 on the micro:bit */
#define SUPPLY_PINS 0xe000
/* Pins 4, 5, 6, 7, 8, 9, 10, 11, 12 on the micro:bit */
#define GROUND_PINS 0x1ff0

void led_matrix_init();
void led_matrix_turn_on();
void led_matrix_turn_off();

#endif