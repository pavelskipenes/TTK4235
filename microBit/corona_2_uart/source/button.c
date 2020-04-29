#include "button.h"
#include "gpio.h"

#define BUTTON_A_PIN 17
#define BUTTON_B_PIN 26
#define INPUT 1
#define OUTPUT 0

void button_init(){
    GPIO->PIN_CNF[BUTTON_A_PIN] = OUTPUT;
    GPIO->PIN_CNF[BUTTON_B_PIN] = OUTPUT;
}

uint8_t button_a_pressed(){
    return !(GPIO->IN & (1 << BUTTON_A_PIN));
}

uint8_t button_b_pressed(){
    return !(GPIO->IN & (1 << BUTTON_B_PIN));
}
