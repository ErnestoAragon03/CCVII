extern void PUT32(unsigned int, unsigned int);
extern unsigned int GET32(unsigned int);

#define UART0_BASE 0x44E09000
#define UART_THR   (UART0_BASE + 0x00)
#define UART_LSR   (UART0_BASE + 0x14)
#define UART_LSR_THRE 0x20

void uart_send(unsigned char x) {
    while ((GET32(UART_LSR) & UART_LSR_THRE) == 0);
    PUT32(UART_THR, x);
}

void uart_puts(const char *s) {
    while (*s) uart_send(*s++);
}

int main(void) {
    while(1){
        for(int i=0; i < 26; i++){
            uart_send('0' + i + 17);
            uart_send('\n');
        }
    }
    return 0;
}
