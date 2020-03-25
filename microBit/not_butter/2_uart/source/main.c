#include "button.h"
#include "uart.h"

int main(){
    button_init();
    uart_init();

    while(1){
        if(button_a_pressed()){
            uart_send('A');
            while(button_a_pressed());
        }
        if(button_b_pressed()){
            uart_send('B');
            while(button_b_pressed());
        }
    }

    return 0;
}
