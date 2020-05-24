#ifndef LED_MATRIX_H
#define LED_MATRIX_H

/**
 * @brief Initialize the LED matrix on the micro:bit.
 * Must be called before @c led_matrix_turn_on and
 * @c led_matrix_turn_off are used.
 */
void led_matrix_init();

/**
 * @brief Turn the micro:bit LED matrix on.
 */
void led_matrix_turn_on();

/**
 * @brief Turn the micro:bit LED matrix off.
 */
void led_matrix_turn_off();

#endif
