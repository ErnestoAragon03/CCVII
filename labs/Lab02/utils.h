#ifndef UTILS_H
#define UTILS_H

int uart_scaled_atoi(const char *s, int scale);

void uart_scaled_itoa(int num, char *buffer, int scale);

int uart_atoi(const char *s);

void uart_itoa(int num, char *buffer);

float uart_atof(const char *s);

void uart_ftoa(float num, char *buffer);

#endif