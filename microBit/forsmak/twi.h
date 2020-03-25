#ifndef TWI_H
#define TWI_H
#include <stdint.h>

/**
 * @brief Initialize a TWI interface on the micro:bit
 * with parameters that work for the micro:bit hardware.
 * Must be called before @c twi_read and @c twi_write
 * are used.
 *
 * @warning The nRF51822 has two TWI interfaces. Only
 * one will be initialized by this call.
 */
void twi_init();


/**
 * @brief Read @p registers_to_read number of registers
 * from a slave with address @p slave_address, starting
 * from the register with address @p start_register, and
 * place the read data into @p data_buffer.
 *
 * @param slave_address Address of the slave to read from.
 * @param start_register Address of start register.
 * @param registers_to_read Number of registers to read.
 * @param data_buffer Buffer to place read data into.
 *
 * @warning @p data_buffer must be at least of size
 * @p registers_to_read to ensure safe operation.
 */
void twi_read(
    uint8_t slave_address,
    uint8_t start_register,
    uint8_t registers_to_read,
    uint8_t * data_buffer
);

/**
 * @brief Write @p registers_to_write number of bytes
 * taken from @p data_buffer into a slave with address
 * @p slave_address, starting at the register address
 * @p start_register.
 *
 * @param slave_address Address of the slave to write to.
 * @param start_register Address of start register.
 * @param registers_to_write Number of registers to write.
 * @param data_buffer Buffer to write data from.
 *
 * @warning @p data_buffer must be at least of size
 * @p registers_to_write to ensure safe operation.
 */
void twi_write(
    uint8_t slave_address,
    uint8_t start_register,
    uint8_t registers_to_write,
    uint8_t * data_buffer
);

#endif
