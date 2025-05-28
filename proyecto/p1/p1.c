#include "../os/uart.h"
extern void uart_send(unsigned char);

unsigned int get_pc(void) {
        unsigned int pc;
        asm volatile ("mov %0, pc" : "=r"(pc));
        return pc;
}

void main_p1(void) {
    while(1){
        uart_send('0' );
        uart_send('\n');
        uart_send('1' );
        uart_send('\n');
        uart_send('2' );
        uart_send('\n');
        uart_send('3' );
        uart_send('\n');
        uart_send('4' );
        uart_send('\n');
        uart_send('5' );
        uart_send('\n');
        uart_send('6' );
        uart_send('\n');
        uart_send('7' );
        uart_send('\n');
        uart_send('8' );
        uart_send('\n');
        uart_send('9' );
        uart_send('\n');
        uart_send('P');uart_send('I');uart_send('P');uart_send('I');
        for (int j=0; j < 10000000; j++); // Esperar un poco
    }
}