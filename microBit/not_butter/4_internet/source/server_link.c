#include "server_link.h"
#include "timer.h"
#include "uart.h"
#include "led_matrix.h"

/* This is stuff you take from the ARMv6-M technical */
/* specification. You got to love datasheets :) */
#define p_AIRCR ((uint32_t *)0xe000ed0c)
#define p_NVIC_ISER ((uint32_t *)(0xe000e100))
#define p_NVIC_ICER ((uint32_t *)(0xe000e180))
#define TIMER0_INTERRUPT 0x0100
#define REQUEST_RESET 0x0004
#define AIRCR_KEY 0x05fa0000

#define MODE_TIMER 0
#define BITMODE_24_BIT 2
#define FREQUENCY_10HZ 1600000
#define CLEAR_ON_COMPARE0 1
#define INTERRUPT_ON_COMPARE0 0x010000

#define STX 0x02
#define ETX 0x03

#define SIGN_ON 0x11
#define STATE 0x12

#define COMMAND_RESET 0x21
#define COMMAND_MATRIX_ON 0x22
#define COMMAND_MATRIX_OFF 0x23

static uint8_t m_buffer[16];
static uint8_t m_command;
static const ServerLinkState * mp_state;

static void send_packet(uint8_t packet_size){
    for(uint8_t i = 0; i < packet_size; i++){
        uart_send(m_buffer[i]);
    }
}

static void send_sign_on(const char * student_id){
    m_buffer[0] = STX;
    m_buffer[1] = SIGN_ON;

    for(uint8_t i = 0; i < 6; i++){
        m_buffer[2 + i] = student_id[i];
    }

    m_buffer[8] = ETX;

    send_packet(9);
}

static void configure_timer(){
    TIMER0->STOP = 1;
    TIMER0->CLEAR = 1;

    TIMER0->MODE = MODE_TIMER;
    TIMER0->BITMODE = BITMODE_24_BIT;
    TIMER0->SHORTS = CLEAR_ON_COMPARE0;
    TIMER0->PRESCALER = 0;

    TIMER0->INTENSET = INTERRUPT_ON_COMPARE0;

    TIMER0->CC[0] = FREQUENCY_10HZ;

    TIMER0->START = 1;
}

void encode_int16(uint8_t * buffer, int16_t number){
    if(number < 0){
        buffer[0] = 0x01;
        number = -number;
    }
    else{
        buffer[0] = 0x00;
    }
    buffer[1] = (uint8_t)(number >> 8);
    buffer[2] = (uint8_t)(number);
}

void TIMER0_IRQHandler(){
    *p_NVIC_ICER = TIMER0_INTERRUPT;

    if(uart_receive(&m_command)){
        switch(m_command){
            case COMMAND_RESET:
                *p_AIRCR = AIRCR_KEY | REQUEST_RESET;
                break;

            case COMMAND_MATRIX_ON:
                led_matrix_turn_on();
                break;

            case COMMAND_MATRIX_OFF:
                led_matrix_turn_off();
                break;

            default:
                break;
        }
    }

    m_buffer[ 0] = STX;
    m_buffer[ 1] = STATE;
    m_buffer[ 2] = (mp_state->button_a_active ? 0x01 : 0x00);
    m_buffer[ 3] = (mp_state->button_b_active ? 0x01 : 0x00);
    m_buffer[ 4] = (uint8_t)(mp_state->accelerometer_x >> 8);
    m_buffer[ 5] = (uint8_t)(mp_state->accelerometer_x);
    m_buffer[ 6] = (uint8_t)(mp_state->accelerometer_y >> 8);
    m_buffer[ 7] = (uint8_t)(mp_state->accelerometer_y);
    m_buffer[ 8] = (uint8_t)(mp_state->accelerometer_z >> 8);
    m_buffer[ 9] = (uint8_t)(mp_state->accelerometer_z);
    m_buffer[10] = ETX;

    send_packet(11);

    TIMER0->COMPARE[0] = 0;

    *p_NVIC_ISER = TIMER0_INTERRUPT;
}

void server_link_init(const char * student_id, const ServerLinkState * p_state){
    mp_state = p_state;

    /* Await confirmation */
    send_sign_on(student_id);

    *p_NVIC_ISER = TIMER0_INTERRUPT;

    configure_timer();
}
