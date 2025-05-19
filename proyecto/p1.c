#include "uart.h"

void main(void) {
    PRINT(">>> Iniciado P1 <<<\n");

    int i = 0;
    while (1) {
        uart_send('0' + (i % 10));
        uart_send('\n');
        i++;
        for (volatile int j = 0; j < 1000000; j++);
    }
}
