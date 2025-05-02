#include "os.h"

// Base real de UART0 en BeagleBone Black (AM335x)
#define UART0_BASE 0x44E09000

// Offsets
#define UART_THR 0x00   // Transmit Holding Register
#define UART_RHR 0x00   // Receive Holding Register
#define UART_LSR 0x14   // Line Status Register

// Bits del Line Status Register
#define UART_LSR_THRE 0x20  // Transmitter Holding Register Empty
#define UART_LSR_DR   0x01  // Data Ready

volatile unsigned int *const UART0 = (unsigned int *)UART0_BASE;

// Enviar un caracter por UART
void uart_putc(char c) {
    // Esperar a que el buffer de transmisión esté vacío
    while (!(UART0[UART_LSR/4] & UART_LSR_THRE));
    UART0[UART_THR/4] = (unsigned int)c;
}

// Recibir un caracter por UART
char uart_getc() {
    // Esperar hasta que haya un dato disponible
    while (!(UART0[UART_LSR/4] & UART_LSR_DR));
    return (char)(UART0[UART_RHR/4] & 0xFF);
}

// Enviar una cadena por UART
void uart_puts(const char *s) {
    while (*s) {
        uart_putc(*s++);
    }
}

// Recibir una cadena por UART (hasta Enter) 
void uart_gets_input(char *buffer, int max_length) {
    int i = 0;
    char c;
    while (i < max_length - 1) {
        c = uart_getc();
        if (c == '\n' || c == '\r') {
            uart_putc('\n'); // Hacer eco
            break;
        }
        uart_putc(c); // Hacer eco
        buffer[i++] = c;
    }
    buffer[i] = '\0';
}

// Funciones vacías placeholder (temporales)
void timer_setup(int miliseconds) {
    // No implementado todavía en BeagleBone
}

void enable_timer_irq() {
    // No implementado todavía en BeagleBone
}

unsigned int get_timer_value() {
    // No hay timer real implementado, devolvemos valor fijo
    return 42;
}

void timer_isr() {
    // No interrupciones todavía
}