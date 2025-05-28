#ifndef UART_H
#define UART_H

#define UART0_BASE     0x44E09000
#define UART_THR       (UART0_BASE + 0x00)
#define UART_LSR       (UART0_BASE + 0x14)
#define UART_LSR_THRE  0x20

void uart_send(unsigned char x);
void uart_decimal(unsigned int value);
void uart_hex(unsigned int value);
void PRINT(const char *s);

#endif