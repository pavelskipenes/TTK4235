#include "button.h"
#include "led_matrix.h"
#include "uart.h"
#include "accelerometer.h"
#include "print.h"
#include "server_link.h"

ServerLinkState m_state;

int main(){
    button_init();
    led_matrix_init();
    uart_init();
    accelerometer_init();
    server_link_init("??????", &m_state);

    while(1){
        m_state.button_a_active = button_a_pressed();
        m_state.button_b_active = button_b_pressed();
        accelerometer_read(
            &m_state.accelerometer_x,
            &m_state.accelerometer_y,
            &m_state.accelerometer_z
        );
    }

    return 0;
}
