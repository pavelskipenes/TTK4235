#include "accelerometer.h"
#include "twi.h"

#define ACCELEROMETER_ADDRESS 0x19

#define CTRL_REG1_A 0x20
#define CTRL_REG2_A 0x21
#define CTRL_REG3_A 0x22
#define CTRL_REG4_A 0x23
#define CTRL_REG5_A 0x24
#define CTRL_REG6_A 0x25

#define XYZ_WITH_AUTO_INCREMENT 0xa8

#define ENABLE_XYZ 0x07
#define DATA_RATE_100_HZ 0x50
#define DISABLE_HIGH_PASS 0x00
#define DISABLE_INTERRUPTS 0x00
#define FULL_SCALE_2G 0x00
#define HIGH_RES_ON 0x08
#define FIFO_BOOT_NORMAL_MODE 0x00
#define DISABLE_BOOT_PIN 0x00

static void set_reg(uint8_t reg, uint8_t value){
    twi_write(ACCELEROMETER_ADDRESS, reg, 1, &value);
}

static void sign_extend_12_bit(int16_t * p_number){
    uint16_t * p_raw_data = (uint16_t *)(p_number);
    if(*p_raw_data & (1 << 11)){
        *p_raw_data |= 0xf000;
    }
}

void accelerometer_init(){
    twi_init();
    set_reg(CTRL_REG1_A, ENABLE_XYZ | DATA_RATE_100_HZ);
    set_reg(CTRL_REG2_A, DISABLE_HIGH_PASS);
    set_reg(CTRL_REG3_A, DISABLE_INTERRUPTS);
    set_reg(CTRL_REG4_A, FULL_SCALE_2G | HIGH_RES_ON);
    set_reg(CTRL_REG5_A, FIFO_BOOT_NORMAL_MODE);
    set_reg(CTRL_REG6_A, DISABLE_BOOT_PIN);
}

void accelerometer_read(int16_t * p_x, int16_t * p_y, int16_t * p_z){
    uint8_t data[6];
    twi_read(ACCELEROMETER_ADDRESS, XYZ_WITH_AUTO_INCREMENT, 6, data);

    *p_x = ((data[1] << 8) | data[0]) >> 4;
    *p_y = ((data[3] << 8) | data[2]) >> 4;
    *p_z = ((data[5] << 8) | data[4]) >> 4;

    sign_extend_12_bit(p_x);
    sign_extend_12_bit(p_y);
    sign_extend_12_bit(p_z);
}
