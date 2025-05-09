extern void PRINT(const char *s);
extern void uart_send(unsigned char);
extern unsigned int GET32(unsigned int);

//para debuggear el timer
#define DMTIMER_TCRR (*(volatile unsigned int *)(0x48040000 + 0x3C)) 
#define DMTIMER_IRQSTATUS (*(volatile unsigned int *)(0x48040000 + 0x28))
//para debuggear INTC
#define INTC_SIR_IRQ      (*(volatile unsigned int *)(0x48200000 + 0x40))

void uart_hex(unsigned int val);

void app_main(void) {
    PRINT("Iniciando app_main (modo debug)...\n");

    while (1) {
        

        unsigned int irq = GET32((unsigned int)&DMTIMER_IRQSTATUS);
        for (int i = 0; i < 10; i++) {
            if (irq & (1 << 1)) {//si se habilita la interrupcion de overflow
                PRINT("TCRR: ");
                uart_hex(GET32((unsigned int)&DMTIMER_TCRR));
                uart_send('\n');
                PRINT("Timer Overflow detectado (IRQSTATUS)\n");
            }
            uart_send('0' + i);
            uart_send('\n');
        }

    }

}
