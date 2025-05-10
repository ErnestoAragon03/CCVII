#include "uart.h"

extern void PUT32(unsigned int, unsigned int);
extern unsigned int GET32(unsigned int);

void uart_send(unsigned char x) {
    while ((GET32(UART_LSR) & UART_LSR_THRE) == 0);
    PUT32(UART_THR, x);
}
void uart_hex(unsigned int value) {
    for (int i = 7; i >= 0; i--) {
        unsigned int nibble = (value >> (i * 4)) & 0xF;
        uart_send(nibble > 9 ? ('A' + nibble - 10) : ('0' + nibble));
    }
}


void PRINT(const char *s) {
    while (*s) uart_send(*s++);
}