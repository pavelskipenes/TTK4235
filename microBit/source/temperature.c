#include "temperature.h"
#include "uart.h"
#include <stdint.h>

#define TEMPERATURE ((NRF_TEMPERATURE_REG*)0x4000C000)

typedef struct {
    // tasks
	volatile uint32_t START;
	volatile uint32_t STOP;
    volatile uint32_t RESERVED0[62];
    //events
	volatile uint32_t DATARDY;
    volatile uint32_t RESERVED1[127];
    // registers
	volatile uint32_t INTEN;
	volatile uint32_t INTENSET;
	volatile uint32_t INTENCLR;
    volatile uint32_t RESERVED2[127];
	volatile uint32_t TEMP;
} NRF_TEMPERATURE_REG;

void temperature_stream(){
    uart_init();

    char buffer[10];
    while(1){
        TEMPERATURE->START = 1;
        while(!TEMPERATURE->DATARDY);
        uart_send_message(itoa(TEMPERATURE->TEMP,buffer,10));

    }
}