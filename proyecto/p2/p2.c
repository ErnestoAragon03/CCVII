#include "../os/uart.h"
extern void uart_send(unsigned char);

unsigned int get_pc1(void) {
        unsigned int pc;
        asm volatile ("mov %0, pc" : "=r"(pc));
        return pc;
}
void main_p2(void) {
    while(1){
        uart_send('A');
        uart_send('\n');
        uart_send('B');
        uart_send('\n');
        uart_send('C');
        uart_send('\n');
        uart_send('D');
        uart_send('\n');
        uart_send('E');
        uart_send('\n');
        uart_send('F');
        uart_send('\n');
        uart_send('G');
        uart_send('\n');
        uart_send('H');
        uart_send('\n');
        uart_send('I');
        uart_send('\n');
        uart_send('J');
        uart_send('\n');
        uart_send('K');
        uart_send('\n');
        uart_send('L');
        uart_send('\n');
        uart_send('M');
        uart_send('\n');
        uart_send('N');
        uart_send('\n');
        uart_send('O');
        uart_send('\n');
        uart_send('P');
        uart_send('\n');
        uart_send('Q');
        uart_send('\n');
        uart_send('R');
        uart_send('\n');
        uart_send('S');
        uart_send('\n');
        uart_send('T');
        uart_send('\n');
        uart_send('U');
        uart_send('\n');
        uart_send('V');
        uart_send('\n');
        uart_send('W');
        uart_send('\n');
        uart_send('X');
        uart_send('\n');
        uart_send('Y');
        uart_send('\n');
        uart_send('Z');
        uart_send('\n');
        for (int j=0; j < 10000000; j++); // Esperar un poco
    }

}
