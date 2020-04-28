#include "button.h"
#include "uart.h"

int main(){
    button_init();
    uart_init();

    while(1){
        if(button_a_pressed()){
            uart_send_message("Button A has been pressed!");
            while(button_a_pressed());
        }
        if(button_b_pressed()){
            uart_send_message("Button B has been pressed!");
            while(button_b_pressed());
        }

    }

    return 0;
}
