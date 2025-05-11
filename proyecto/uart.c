#include "uart.h"

extern void PUT32(unsigned int, unsigned int);
extern unsigned int GET32(unsigned int);

void uart_send(unsigned char x) {
    while ((GET32(UART_LSR) & UART_LSR_THRE) == 0);
    PUT32(UART_THR, x);
}

void uart_decimal(unsigned int value) {
    char buffer[10]; // Máximo para un uint32_t: 4294967295 (10 dígitos)
    int i = 0;

    // Caso especial para 0
    if (value == 0) {
        uart_send('0');
        return;
    }

    // Convertir a decimal (en orden inverso)
    while (value > 0) {
        buffer[i++] = '0' + (value % 10);
        value /= 10;
    }

    // Enviar en orden correcto
    while (i-- > 0) {
        uart_send(buffer[i]);
    }
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