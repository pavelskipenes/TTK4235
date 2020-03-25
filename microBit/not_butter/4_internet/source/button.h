#ifndef BUTTON_H
#define BUTTON_H
#include <stdint.h>

/**
 * @brief Initialize the buttons on the micro:bit.
 * Must be called before @c button_a_pressed and
 * @c button_b_pressed are used.
 */
void button_init();

/**
 * @brief Check if button A is pressed on the micro:bit.
 *
 * @return 1 if button A is pressed; 0 otherwise.
 */
uint8_t button_a_pressed();

/**
 * @brief Check if button B is pressed on the micro:bit.
 *
 * @return 1 if button B is pressed; 0 otherwise.
 */
uint8_t button_b_pressed();

#endif
