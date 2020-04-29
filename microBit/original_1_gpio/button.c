#include "button.h"
#include "gpio.h"

void button_init(){
    GPIO->PIN_CNF[BUTTON_A_PIN] = BUTTON_OUTPUT;
    GPIO->PIN_CNF[BUTTON_B_PIN] = BUTTON_OUTPUT;
}

uint8_t button_a_pressed(){
    return !(GPIO->IN & 1 << BUTTON_A_PIN);
}

uint8_t button_b_pressed(){
    return !(GPIO->IN & 1 << BUTTON_B_PIN);
}
