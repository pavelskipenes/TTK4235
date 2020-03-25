#ifndef PRINT_H
#define PRINT_H
#include <stdint.h>

/**
 * @brief Lightweight function that mimics a subset of fprintf, working
 * only with integers, but able to print in both decimal and hexadecimal
 * representations. Supports signs and requested field widths.
 *
 * To inject numbers into a format string, a '%' is used.This is followed
 * by an optional field width, written as a decimal number.
 * Lastly, either a 'd', or an 'x' must follow. A 'd' requests that the
 * number be printed using decimal notation, while an 'x' requests
 * hexadecimal notation. The notation specifier is case sensitive.
 *
 * EXAMPLE USAGE:
 * print_format(uart_send, "Number: %d\n\r", 42);
 * Outputs "Number: 42"
 *
 * print_format(uart_send, "Number: %6d\n\r", 175);
 * Outputs "Number: ___175"
 * (Underscores are not printed, only demonstrates requested print width)
 *
 * print_format(uart_send, "Number: %x\n\r", -45067);
 * Outputs "Number: -b00b"
 *
 * If the required print length exceeds the requested print length,
 * the required print length will be used:
 * print_format(uart_send, "Number: %2d\n\r", 212);
 * Outputs: "Number: 212"
 *
 * @param put_func Function pointer to a function putting a single
 * byte to a device specified by the function.
 * @param format_string A formatted string to be printed, possibly
 * with injected '%'s to represent a number.
 * @param ... A variable list of numbers, equal in length to the
 * number of injected '%'s in @p format_string.
 */
void print_format(void (*put_func)(uint8_t), const char * format_string, ...);

#endif
