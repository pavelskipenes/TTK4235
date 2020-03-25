#include "button.h"
#include "gpio.h"

/* Fyll inn disse to */
#define BUTTON_A_PIN ??
#define BUTTON_B_PIN ??

void button_init(){
    GPIO->PIN_CNF[BUTTON_A_PIN] = 0;
    GPIO->PIN_CNF[BUTTON_B_PIN] = 0;
}

uint8_t button_a_pressed(){
    /* Fill me up with hot code */
}

uint8_t button_b_pressed(){
    /* Fill me up with hot code */
}
