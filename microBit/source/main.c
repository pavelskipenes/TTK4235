#include <stdint.h>

#include "button.h"
#include "uart.h"
#include "button.h"
#include "led_matrix.h"
#include "print.h"
#include "accelerometer.h"
#include "server_link.h"
#include "temperature.h"

static void gpio_assignment();
static void uart_assignment();
static void twi_assignment();
static void iot_assignment();


int main(){

    //gpio_assignment();
    //uart_assignment();
    //twi_assignment();
    //iot_assignment();
    temperature_stream();
    return 0;
}

static void gpio_assignment(){
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
}

static void uart_assignment(){
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
}
static void twi_assignment(){
    uart_init();
    accelerometer_init();

    int16_t x, y, z;

    while(1){
        accelerometer_read(&x, &y, &z);
        print_format(uart_send, "X: %6d Y: %6d Z: %6d\n\r", x, y, z);
    }
}

static void iot_assignment(){
    ServerLinkState m_state;

    button_init();
    led_matrix_init();
    uart_init();
    accelerometer_init();
    server_link_init("a354a4", &m_state);

    while(1){
        m_state.button_a_active = button_a_pressed();
        m_state.button_b_active = button_b_pressed();
        accelerometer_read(
            &m_state.accelerometer_x,
            &m_state.accelerometer_y,
            &m_state.accelerometer_z
        );
    }
}