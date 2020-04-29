#ifndef TIMER_H
#define TIMER_H
#include <stdint.h>

#define TIMER0 ((NRF_TIMER_REG*)0x40008000)
#define TIMER1 ((NRF_TIMER_REG*)0x40009000)
#define TIMER2 ((NRF_TIMER_REG*)0x4000a000)

typedef struct {
    /* Tasks */
    volatile uint32_t START;
    volatile uint32_t STOP;
    volatile uint32_t COUNT;
    volatile uint32_t CLEAR;
    volatile uint32_t SHUTDOWN;
    volatile uint32_t RESERVED0[11];
    volatile uint32_t CAPTURE[4];
    /* Events */
    volatile uint32_t RESERVED1[60];
    volatile uint32_t COMPARE[4];
    /* Registers */
    volatile uint32_t RESERVED2[44];
    volatile uint32_t SHORTS;
    volatile uint32_t RESERVED3[64];
    volatile uint32_t INTENSET;
    volatile uint32_t INTENCLR;
    volatile uint32_t RESERVED4[126];
    volatile uint32_t MODE;
    volatile uint32_t BITMODE;
    volatile uint32_t RESERVED5;
    volatile uint32_t PRESCALER;
    volatile uint32_t RESERVED6[11];
    volatile uint32_t CC[4];
} NRF_TIMER_REG;

#endif
