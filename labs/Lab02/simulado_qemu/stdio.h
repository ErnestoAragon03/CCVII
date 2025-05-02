//Declaramos el nuevo PRINT Y READ

#ifndef STDIO_H
#define STDIO_H


void PRINT(const char *format, ...); // ... puede recibir argumentos variables

void READ(const char *format, ...); 

void uart_itoa(int num, char *buffer);





#endif // STDIO_H
