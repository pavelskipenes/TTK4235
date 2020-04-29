#include "button.h"
#include "led_matrix.h"

int main(){
    button_init();
    led_matrix_init();

    while(1){
        if(button_a_pressed()){
            led_matrix_turn_on();
        }
        if(button_b_pressed()){
            led_matrix_turn_off();
        }
    }

    return 0;
}
