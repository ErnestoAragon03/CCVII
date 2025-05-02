#ifndef _UTILS_H_
#define _UTILS_H_

void uart_putc(char c);
void uart_puts(const char* s);
char uart_getc();

void uart_itoa(int num, char *buffer);
int uart_atoi(const char *s);
void uart_gets_input(char *buffer, int maxlen);

#endif
