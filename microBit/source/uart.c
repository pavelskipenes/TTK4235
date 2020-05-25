#include "uart.h"
#include "gpio.h"

#define PIN_TX 24
#define PIN_RX 25
#define PIN_DISABLE 0xFFFFFFFF

#define BAUD_1200 0x0004F000
#define BAUD_2400 0x0009D000
#define BAUD_4900 0x0013B000
#define BAUD_9600 0x00275000
#define BAUD_14400 0x003B0000
#define BAUD_19200 0x004EA000
#define BAUD_28800 0x0075F000
#define BAUD_48400 0x009D5000
#define BAUD_57600 0x00EBF000
#define BAUD_76800 0x013A9000
#define BAUD_115200 0x01D7E000
#define BAUD_230400 0x03AFB000
#define BAUD_250000 0x04000000
#define BAUD_460800 0x075F7000
#define BAUD_921600 0x0EBEDFA4
#define BAUD_1M 0x10000000

#define UART_DISABLE 0
#define UART_ENABLE 4

#define UART ((NRF_UART_REG*)0x40002000)

#define INPUT 0
#define OUTPUT 1

typedef struct {
    /* Tasks */
	volatile uint32_t STARTRX;
	volatile uint32_t STOPRX;
	volatile uint32_t STARTTX;
	volatile uint32_t STOPTX;
	volatile uint32_t RESERVED0[3];
	volatile uint32_t SUSPEND;
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

    GPIO->PIN_CNF[PIN_TX] = OUTPUT;
    GPIO->PIN_CNF[PIN_RX] = INPUT;
	
	UART->PSELTXD = PIN_TX;
	UART->PSELRXD = PIN_RX;
	UART->PSELRTS = PIN_DISABLE;
	UART->PSELCTS = PIN_DISABLE;

	UART->BAUDRATE = BAUD_9600;

    UART->ENABLE = UART_ENABLE;

    UART->STARTRX = 1;
}

void uart_send(uint8_t byte){
	// Section 29.4
	UART->STARTTX = 1;
	UART->TXD = byte;
	
	while(!UART->TXDRDY);
	UART->TXDRDY = 0;

	UART->STOPTX = 1;
}

void uart_send_message(const char* message){
	int i = 0;
	while(message[i] != '\0'){
		uart_send((uint8_t)message[i]);
		i++;
	}

	uart_send('\n');
	uart_send('\r');
}

void uart_send_number(int number){
	char buffer[11]; // maximum integer size + /0
	uart_send_message(itoa(number,buffer,10));
}

uint8_t uart_receive(uint8_t * p_byte){
	// Section 29.5
    if(!UART->RXDRDY)
        return 0;

    UART->RXDRDY = 0;
    *p_byte = UART->RXD;
    return 1;
}
