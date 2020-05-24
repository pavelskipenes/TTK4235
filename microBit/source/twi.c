#include "twi.h"
#include "gpio.h"
#include "timer.h"

#define PIN_SCL 0
#define PIN_SDA 30
#define STANDARD_0_DISCONNECT_1 0x0600
#define TWI_100_KBPS 0x01980000
#define TWI_DISABLE 0
#define TWI_ENABLE 5

#define MODE_TIMER 0
#define BITMODE_24_BIT 2
#define FREQUENCY_2HZ 8000000
#define FREQUENCY_100HZ 160000
#define CLEAR_ON_COMPARE0 1

#define TWI0 ((NRF_TWI_REG*)0x40003000)

typedef struct {
	// Tasks
	volatile uint32_t STARTRX;
	volatile uint32_t RESERVED0;
	volatile uint32_t STARTTX;
	volatile uint32_t RESERVED1[2];
	volatile uint32_t STOP;
	volatile uint32_t RESERVED2;
	volatile uint32_t SUSPEND;
	volatile uint32_t RESUME;
	// Events
	volatile uint32_t RESERVED3[56];
	volatile uint32_t STOPPED;
	volatile uint32_t RXDREADY;
	volatile uint32_t RESERVED4[4];
	volatile uint32_t TXDSENT;
	volatile uint32_t RESERVED5;
	volatile uint32_t ERROR;
	volatile uint32_t RESERVED6[4];
	volatile uint32_t BB;
	// Registers
	volatile uint32_t RESERVED7[49];
	volatile uint32_t SHORTS;
	volatile uint32_t RESERVED8[63];
	volatile uint32_t INTEN;
	volatile uint32_t INTENSET;
	volatile uint32_t INTENCLR;
	volatile uint32_t RESERVED9[110];
	volatile uint32_t ERRORSRC;
	volatile uint32_t RESERVED10[14];
	volatile uint32_t ENABLE;
	volatile uint32_t RESERVED11;
	volatile uint32_t PSELSCL;
	volatile uint32_t PSELSDA;
	volatile uint32_t RESERVED12[2];
	volatile uint32_t RXD;
	volatile uint32_t TXD;
	volatile uint32_t RESERVED13;
	volatile uint32_t FREQUENCY;
	volatile uint32_t RESERVED14[24];
	volatile uint32_t ADDRESS;
} NRF_TWI_REG;

static void start_timer(uint32_t frequency){
    TIMER1->STOP = 1;
    TIMER1->CLEAR = 1;

    TIMER1->MODE = MODE_TIMER;
    TIMER1->BITMODE = BITMODE_24_BIT;
    TIMER1->SHORTS = CLEAR_ON_COMPARE0;
    TIMER1->COMPARE[0] = 0;
    TIMER1->PRESCALER = 0;
    TIMER1->CC[0] = frequency;

    TIMER1->START = 1;
}

static void start_watchdog(){
    start_timer(FREQUENCY_2HZ);
}

static uint8_t watchdog_expired(){
    return TIMER1->COMPARE[0];
}

static void recover_bus_from_halt(){
    TWI0->ENABLE = 0;

    GPIO->PIN_CNF[PIN_SCL] = 1;
    GPIO->PIN_CNF[PIN_SDA] = 1;

    GPIO->OUTSET = (1 << PIN_SCL) | (1 << PIN_SDA);

    start_timer(FREQUENCY_100HZ);

    for(uint8_t tick = 0; tick < 9; tick++){
        while(!TIMER1->COMPARE[0]);
        GPIO->OUTCLR = (1 << PIN_SCL);
        TIMER1->COMPARE[0] = 0;

        while(!TIMER1->COMPARE[0]);
        GPIO->OUTSET = (1 << PIN_SCL);
        TIMER1->COMPARE[0] = 0;
    }

    TIMER1->STOP = 1;

    twi_init();
}

static uint8_t twi_read_with_watchdog(
    uint8_t slave_address,
    uint8_t start_register,
    uint8_t registers_to_read,
    uint8_t * data_buffer
){
    TWI0->ADDRESS = slave_address;
    TWI0->STARTTX = 1;

    TWI0->TXDSENT = 0;
    TWI0->TXD = start_register;
    start_watchdog();
    while(!TWI0->TXDSENT){
        if(watchdog_expired()){
            return 0;
        }
    }

    /* Unfortunately, the micro:bit has certain timing issues */
    /* that we must take into consideration. To prevent the */
    /* compiler from removing code that "does nothing", we */
    /* must resort to a little bit of inline assembly */
    for(uint8_t sync = 0; sync < 10; sync++){
        __asm("nop");
    }

    TWI0->RXDREADY = 0;
    TWI0->STARTRX = 1;

    for(uint8_t i = 0; i < registers_to_read - 1; i++){
        start_watchdog();
        while(!TWI0->RXDREADY){
            if(watchdog_expired()){
                return 0;
            }
        }
        TWI0->RXDREADY = 0;

        data_buffer[i] = TWI0->RXD;
    }

    TWI0->STOP = 1;
    start_watchdog();
    while(!TWI0->RXDREADY){
        if(watchdog_expired()){
            return 0;
        }
    }

    data_buffer[registers_to_read - 1] = TWI0->RXD;
    return 1;
}

static uint8_t twi_write_with_watchdog(
    uint8_t slave_address,
    uint8_t start_register,
    uint8_t registers_to_write,
    uint8_t * data_buffer
){
    TWI0->ADDRESS = slave_address;
    TWI0->STARTTX = 1;

    TWI0->TXDSENT = 0;
    TWI0->TXD = start_register;
    start_watchdog();
    while(!TWI0->TXDSENT){
        if(watchdog_expired()){
            return 0;
        }
    }

    for(uint8_t i = 0; i < registers_to_write; i++){
        TWI0->TXDSENT = 0;
        TWI0->TXD = data_buffer[i];
        start_watchdog();
        while(!TWI0->TXDSENT){
            if(watchdog_expired()){
                return 0;
            }
        }
    }

    TWI0->STOP = 1;

    return 1;
}

void twi_init(){
    TWI0->ENABLE = TWI_DISABLE;

    GPIO->PIN_CNF[PIN_SCL] = STANDARD_0_DISCONNECT_1;
    GPIO->PIN_CNF[PIN_SDA] = STANDARD_0_DISCONNECT_1;

    TWI0->RXDREADY = 0;
    TWI0->TXDSENT = 0;
    TWI0->ERROR = 0;

    TWI0->PSELSCL = PIN_SCL;
    TWI0->PSELSDA = PIN_SDA;

    TWI0->FREQUENCY = TWI_100_KBPS;

    TWI0->ENABLE = TWI_ENABLE;
}

void twi_read(
    uint8_t slave_address,
    uint8_t start_register,
    uint8_t registers_to_read,
    uint8_t * data_buffer
){
    while(1){
        uint8_t success = twi_read_with_watchdog(
            slave_address,
            start_register,
            registers_to_read,
            data_buffer
        );

        if(success){
            break;
        }
        else{
            recover_bus_from_halt();
        }
    }
}

void twi_write(
    uint8_t slave_address,
    uint8_t start_register,
    uint8_t registers_to_write,
    uint8_t * data_buffer
){
    while(1){
        uint8_t success = twi_write_with_watchdog(
            slave_address,
            start_register,
            registers_to_write,
            data_buffer
        );

        if(success){
            break;
        }
        else{
            recover_bus_from_halt();
        }
    }
}
