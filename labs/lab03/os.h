#ifndef OS_H
#define OS_H

// Declaraciones de funciones de UART
void uart_putc(char c);
char uart_getc();
void uart_puts(const char *s);
void uart_gets_input(char *buffer, int max_length);
int uart_scaled_atoi(const char *s, int scale);
void uart_scaled_itoa(int num, char *buffer, int scale);
int uart_atoi(const char *s);
void uart_itoa(int num, char *buffer);
float uart_atof(const char *s);
void uart_ftoa(float num, char *buffer);

// Declaraciones de funciones de Timer y VIC
void vic_enable_timer_irq();
void timer_clear_interrupt();
void timer_init();
void vic_disable_timer_irq();
unsigned int get_timer_value();





#endif