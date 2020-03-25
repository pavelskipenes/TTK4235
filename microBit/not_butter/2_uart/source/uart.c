#include "uart.h"
#include "gpio.h"

/* Fyll inn disse */
#define PIN_TX ??
#define PIN_RX ??
#define PIN_DISABLE 0x????????
#define BAUD_9600 0x????????

#define UART_DISABLE 0
#define UART_ENABLE 4

/* Fyll inn riktig startadresse */
#define UART ((NRF_UART_REG*)0x????????)

typedef struct {
    /* Tasks */
    /* Her skal det fylles inn seks felter */


	/* Events */
	volatile uint32_t RESERVED1[56];
	volatile uint32_t CTS;
	volatile uint32_t NCTS;
	volatile uint32_t RXDRDY;
	volatile uint32_t RESERVED2[4];
	volatile uint32_t TXDRDY;
	volatile uint32_t RESERVED3;
	volatile uint32_t ERROR;
	volatile uint32_t RESERVED4[7];
	volatile uint32_t RXTO;
	/* Registers */
	volatile uint32_t RESERVED5[110];
	volatile uint32_t INTEN;
	volatile uint32_t INTENSET;
	volatile uint32_t INTENCLR;
	volatile uint32_t RESERVED6[93];
	volatile uint32_t ERRORSRC;
	volatile uint32_t RESERVED7[31];
	volatile uint32_t ENABLE;
	volatile uint32_t RESERVED8;
	volatile uint32_t PSELRTS;
	volatile uint32_t PSELTXD;
	volatile uint32_t PSELCTS;
	volatile uint32_t PSELRXD;
	volatile uint32_t RXD;
	volatile uint32_t TXD;
	volatile uint32_t RESERVED9;
	volatile uint32_t BAUDRATE;
	volatile uint32_t RESERVED10[17];
	volatile uint32_t CONFIG;
} NRF_UART_REG;

void uart_init(){
    UART->ENABLE = UART_DISABLE;

    GPIO->PIN_CNF[PIN_TX] = 1;
    GPIO->PIN_CNF[PIN_RX] = 0;

    /* Konfigurer "Pin Select" for RTS, CTS, TXD, og RXD */

    /* Konfigurer Baudhastigheten */

    UART->ENABLE = UART_ENABLE;

    UART->STARTRX = 1;
}

void uart_send(uint8_t byte){
    /* Fyll inn dette */
}

uint8_t uart_receive(uint8_t * p_byte){
    if(!UART->RXDRDY)
        return 0;

    UART->RXDRDY = 0;
    *p_byte = UART->RXD;
    return 1;
}
