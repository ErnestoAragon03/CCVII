#include "uart.h"

void main(void) {
    PRINT(">>>P2<<<\n");

    char letra = 'a';
    int limite = 20;  // 👈 solo 10 letras

    while (limite--) {
        uart_send(letra++);
        uart_send('\n');
        if (letra > 'z') letra = 'a';
        for (volatile int j = 0; j < 1000000; j++);
    }

}
