#include "stdio.h"
#include "os.h"
#include <stdarg.h> //manejar argumentos variables en funciones

//va_list → Define la lista de argumentos.
//va_start() → Inicia la lectura de argumentos.
//va_arg() → Obtiene el siguiente argumento.
//va_end() → Finaliza la lectura de argumentos.

#define UART0_BASE 0x101f1000

#define UART_DR      0x00  // Data Register escriben/leen los datos
#define UART_FR      0x18  // Flag Register si el buffer de transmisión está lleno o el de recepción está vacío
#define UART_FR_TXFF 0x20  // Transmit FIFO Full  si el buffer de transmisión está lleno.
#define UART_FR_RXFE 0x10  // Receive FIFO Empty si el buffer de recepción está vacío.
 
volatile unsigned int *const UART0 = (unsigned int *)UART0_BASE; //permite acceder a los registros

// Función PRINT
void PRINT(const char *format, ...) {
    va_list args;
    va_start(args, format);

    while (*format) {
        if (*format == '%') {
            format++;
            switch (*format) {
                case 's': {
                    char *str = va_arg(args, char*);
                    uart_puts(str);
                    break;
                }
                case 'd': {
                    int num = va_arg(args, int);
                    char buffer[16];
                    uart_itoa(num, buffer);
                    uart_puts(buffer);
                    break;
                }
                case 'f': {
                    int num = va_arg(args, int);
                    char buffer[32];
                    uart_scaled_itoa(num, buffer, 100); // Escala para 2 decimales
                    uart_puts(buffer);
                    break;
                }
                default:
                    uart_putc('%');
                    uart_putc(*format);
                    break;
            }
        } else {
            uart_putc(*format);
        }
        format++;
    }

    va_end(args);
}

// Función READ (similar a scanf)
void READ(const char *format, ...) {
    va_list args;
    va_start(args, format);

    while (*format) {
        if (*format == '%') {
            format++;  // Avanzar al especificador
            switch (*format) {
                case 's': {
                    // Cadena
                    char *str = va_arg(args, char*);
                    uart_gets_input(str, 100);  // Lee una línea de entrada
                    break;
                }
                case 'd': {
                    // Entero
                    int *num = va_arg(args, int*);
                    char buffer[16];
                    uart_gets_input(buffer, sizeof(buffer));
                    *num = uart_atoi(buffer);  // Convierte la cadena a entero
                    break;
                }
                case 'f': {
                    // Flotante
                    float *num = va_arg(args, float*);
                    char buffer[32];
                    uart_gets_input(buffer, sizeof(buffer));
                    *num = uart_atof(buffer);  // Convierte la cadena a flotante
                    break;
                }
                default:
                    // Especificador no reconocido
                    break;
            }
        }
        format++;
    }

    va_end(args);
}