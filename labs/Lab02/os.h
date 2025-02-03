#ifndef OS_H
#define OS_H

void uart_putc(char c);

char uart_getc();

void uart_puts(const char *s);

void uart_gets_input(char *buffer, int max_length);

void timer_setup(int miliseconds);

void enable_timer_irq();

void timer_isr();


#endif