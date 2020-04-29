#include "uart.h"
#include "accelerometer.h"
#include "print.h"
#include <stdint.h>

int main(){
    uart_init();
    accelerometer_init();

    int16_t x, y, z;

    while(1){
        accelerometer_read(&x, &y, &z);
        print_format(uart_send, "X: %6d Y: %6d Z: %6d\n\r", x, y, z);
    }

    return 0;
}
