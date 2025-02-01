#ifndef OS_H
#define OS_H

void uart_putc(char c);

void uart_puts(const char *s);

void uart_gets_input(char *buffer, int max_length);

void uart_scaled_itoa(int num, char *buffer, int scale);

int uart_atoi(const char *s);

void uart_itoa(int num, char *buffer);

float uart_atof(const char *s);

#endif