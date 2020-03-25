#include "print.h"
#include <stdarg.h>

static uint8_t get_minimum_width(const char * l){
    uint8_t width = 0;
    while(*l != 'd' && *l != 'x'){
        width *= 10;
        width += (*l - '0');
        l++;
    }
    return width;
}

static uint8_t get_required_width(char radix_format, int32_t number){
    if(number == 0){
        return 1;
    }

    uint8_t width = 0;
    if(number < 0){
        width = 1;
    }

    uint8_t radix = 10;
    if(radix_format == 'x'){
        radix = 16;
    }

    while(number != 0){
        width++;
        number /= radix;
    }

    return width;
}

static void print_number(
    void (*put_func)(uint8_t),
    char radix_format,
    int32_t number
){
    if(number == 0){
        put_func('0');
        return;
    }

    uint8_t radix = 10;
    if(radix_format == 'x'){
        radix = 16;
    }

    uint8_t digits = 0;
    int32_t reverse = 0;
    uint8_t print_negative_sign = 0;

    if(number < 0){
        print_negative_sign = 1;
        number = -number;
    }

    while(number != 0){
        reverse *= radix;
        reverse += (number % radix);
        number /= radix;
        digits++;
    }

    if(print_negative_sign){
        put_func('-');
    }

    while(reverse != 0){
        uint8_t rem = reverse % radix;
        uint8_t put_byte;
        if(rem < 10){
            put_byte = rem + '0';
        }
        else{
            put_byte = rem - 10 + 'a';
        }
        put_func(put_byte);
        reverse /= radix;
        digits--;
    }

    while(digits != 0){
        put_func('0');
        digits--;
    }
}

void print_format(void (*put_func)(uint8_t), const char * format_string, ...){
    va_list p_arg;
    va_start(p_arg, format_string);

    for(char * l = (char *)format_string; *l != '\0'; l++){
        if(*l != '%'){
            put_func(*l);
            continue;
        }

        int32_t number = va_arg(p_arg, int32_t);
        l++;

        uint8_t min_width = get_minimum_width(l);
        while(*l != 'd' && *l != 'x'){
            l++;
        }

        uint8_t req_width = get_required_width(*l, number);

        while(req_width < min_width){
            put_func(' ');
            req_width++;
        }

        print_number(put_func, *l, number);
    }

    va_end(p_arg);
}
