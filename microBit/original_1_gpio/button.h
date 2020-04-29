#ifndef _BUTTON_H_
#define _BUTTON_H_

#include <stdint.h>
#define BUTTON_A_PIN 17
#define BUTTON_B_PIN 26

#define BUTTON_OUTPUT 0
#define BUTTON_INPUT 1

void button_init();
uint8_t button_a_pressed();
uint8_t button_b_pressed();

#endif
