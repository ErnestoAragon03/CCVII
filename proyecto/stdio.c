#include "stdio.h"
#include "utils.h"
#include <stdarg.h>

#define UART0 0x44E09000

void uart_putc(char c) {
    volatile unsigned int* UART_THR = (unsigned int*)(UART0 + 0x00);
    volatile unsigned int* UART_LSR = (unsigned int*)(UART0 + 0x14);
    while (!(*UART_LSR & (1 << 5)));
    *UART_THR = c;
}

void uart_puts(const char* s) {
    while (*s) {
        uart_putc(*s++);
    }
}

char uart_getc() {
    volatile unsigned int* UART_LSR = (unsigned int*)(UART0 + 0x14);
    volatile unsigned int* UART_RHR = (unsigned int*)(UART0 + 0x00);
    while (!(*UART_LSR & 0x01));
    return (char)(*UART_RHR & 0xFF);
}

void PRINT(const char *format, ...) {
    va_list args;
    va_start(args, format);

    while (*format) {
        if (*format == '%') {
            format++;
            switch (*format) {
                case 's': {
                    char *str = va_arg(args, char*);
                    uart_puts(str);
                    break;
                }
                case 'd': {
                    int num = va_arg(args, int);
                    char buffer[16];
                    uart_itoa(num, buffer);
                    uart_puts(buffer);
                    break;
                }
                default:
                    uart_putc('%');
                    uart_putc(*format);
                    break;
            }
        } else {
            uart_putc(*format);
        }
        format++;
    }

    va_end(args);
}

void READ(const char *format, ...) {
    va_list args;
    va_start(args, format);

    while (*format) {
        if (*format == '%') {
            format++;
            switch (*format) {
                case 's': {
                    char *str = va_arg(args, char*);
                    uart_gets_input(str, 100);
                    break;
                }
                case 'd': {
                    int *num = va_arg(args, int*);
                    char buffer[16];
                    uart_gets_input(buffer, sizeof(buffer));
                    *num = uart_atoi(buffer);
                    break;
                }
                default:
                    break;
            }
        }
        format++;
    }

    va_end(args);
}
