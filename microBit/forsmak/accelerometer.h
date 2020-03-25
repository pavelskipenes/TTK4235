#ifndef ACCELEROMETER_H
#define ACCELEROMETER_H
#include <stdint.h>

/**
 * @brief Initialize the micro:bit on-board accelerometer. This
 * call will initialize all underlying resources required for
 * normal accelerometer operation with sane parameters that will
 * work for most use cases.
 */
void accelerometer_init();

/**
 * @brief Read the accelerometer values in X, Y, and Z directions.
 *
 * If the accelerometer has been initialized with a call to
 * @c accelerometer_init, the returned values will be in the range
 * [-2048, 2047], which corresponds to -2g and +2g, respectively.
 *
 * A value of 1024 can be taken to mean 1g in the positive direction.
 *
 * @param p_x Pointer to storage of X reading after call.
 * @param p_y Pointer to storage of Y reading after call.
 * @param p_z Pointer to storage of Z reading after call.
 */
void accelerometer_read(int16_t * p_x, int16_t * p_y, int16_t * p_z);

#endif
