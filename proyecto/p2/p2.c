#include "../os/uart.h"
extern void uart_send(unsigned char);

unsigned int get_pc1(void) {
        unsigned int pc;
        asm volatile ("mov %0, pc" : "=r"(pc));
        return pc;
}
void main_p2(void) {
    while(1){
        for(int i=0; i < 26; i++){
            uart_send('0' + i + 17);
            uart_send('\n');

            for(int j=0; j < 10000000; j++); // Esperar un poco

        }
            unsigned int pc = get_pc1();
            uart_send('P');
            uart_send('C');
            uart_send(':');
            uart_hex(pc);
            uart_send('\n');
    }

}
