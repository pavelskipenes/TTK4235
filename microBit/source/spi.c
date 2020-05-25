#include "spi.h"

#include <stdint.h>

#define SPI0 ((NRF_SPI_REG*)0x40003000)
#define SPI1 ((NRF_SPI_REG*)0x40004000)

#define PIN_DISABLED 0xFFFFFFFF

#define PIN_MOSI 21
#define PIN_SIMO 22
#define PIN_CLOCK 23



typedef struct {
    volatile uint32_t RESERVED1[66];
    // events
	volatile uint32_t READY;
    volatile uint32_t RESERVED2[125];
    // registers 
	volatile uint32_t INTEN; 
	volatile uint32_t INTENSET; 
	volatile uint32_t INTENCLR;
    volatile uint32_t RESERVED3[125];
	volatile uint32_t ENABLE;
    volatile uint32_t RESERVED4[1];
	volatile uint32_t PSELSCK;
	volatile uint32_t PSELMOSI;
	volatile uint32_t PSELMISO;
    volatile uint32_t RESERVED5[1];
	volatile uint32_t RXD; 
	volatile uint32_t TXD;
    volatile uint32_t RESERVED6[1];
	volatile uint32_t FREQUENCY;
    volatile uint32_t RESERVED7[11];
	volatile uint32_t CONFIG; 
} NRF_SPI_REG;

void spi_init(){
    SPI->ENABLE = 0;
    SPI->PSELSCK = PIN_CLOCK;
    SPI->PSELMOSI = PIN_MOSI;
    SPI->PSELSIMO = PIN_SIMO;
    SPI->ENABLE = 1;
}

void spi_send_byte(byte letter){
    SPI->TXD = letter;
}


void spi_read_byte(){
    while(!SPI->READY);
    return SPI->RXD;
}
