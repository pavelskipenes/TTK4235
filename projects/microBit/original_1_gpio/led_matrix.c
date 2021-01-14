#include "led_matrix.h"
#include "gpio.h"

void led_matrix_init(){
    GPIO->DIRSET = SUPPLY_PINS | GROUND_PINS;
    GPIO->OUTCLR = SUPPLY_PINS | GROUND_PINS;
}

void led_matrix_turn_on(){
    GPIO->OUTSET = SUPPLY_PINS;
}

void led_matrix_turn_off(){
    GPIO->OUTCLR = SUPPLY_PINS;
}