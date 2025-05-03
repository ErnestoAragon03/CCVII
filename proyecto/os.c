
//TIMER2
#define DMTIMER2_BASE 0x48040000
#define DMTIMER_TIDR           (*(volatile unsigned int *)(DMTIMER2_BASE + 0x0))
#define DTIMER_IRQ_EOI         (*(volatile unsigned int *)(DMTIMER2_BASE + 0x20))// Timer IRQ End-of-Interrupt Register
#define DMTIMER_IRQSTATUS_RAW  (*(volatile unsigned int *)(DMTIMER2_BASE + 0x24))
#define DMTIMER_IRQSTATUS      (*(volatile unsigned int *)(DMTIMER2_BASE + 0x28))
#define DMTIMER_IRQENABLE_SET  (*(volatile unsigned int *)(DMTIMER2_BASE + 0x2C))//Timer Interrupt Enable Set Register
#define DMTIMER_IRQENABLE_CLR  (*(volatile unsigned int *)(DMTIMER2_BASE + 0x30))// Timer Interrupt Enable Clear Register
#define DMTIMER_TCLR           (*(volatile unsigned int *)(DMTIMER2_BASE + 0x38))//activa o desactiva en cualquier momento al timer
#define DMTIMER_TCRR           (*(volatile unsigned int *)(DMTIMER2_BASE + 0x3C))//contiene el valor del timer
#define DMTIMER_TLDR           (*(volatile unsigned int *)(DMTIMER2_BASE + 0x40))//contiene el valor de carga del timer en modo Auto reload

//INTERRUPCIONES TIMER 2
#define INTC_BASE 0x48200000
#define INTC_SIR_IRQ           (*(volatile unsigned int *)(INTC_BASE + 0x40))//contiene el numero de la interrupcion que ha sido activada
#define INTC_CONTROL           (*(volatile unsigned int *)(INTC_BASE + 0x48))//1h(Write) = NewIrq_Reset IRQ output and enable new IRQ generation
// INTC_MIR_CLEAR2 = INTC_BASE + 0x90 + 0x04  (MIR2 clear)
#define INTC_MIR_CLEAR2 (*(volatile unsigned int *)(INTC_BASE + 0x90 + 0x04))

//UART
extern void PUT32(unsigned int, unsigned int);
extern unsigned int GET32(unsigned int);
extern void os_main(void);
#define UART0_BASE 0x44E09000
#define UART_THR   (UART0_BASE + 0x00)
#define UART_LSR   (UART0_BASE + 0x14)
#define UART_LSR_THRE 0x20

void uart_send(unsigned char x) {
    while ((GET32(UART_LSR) & UART_LSR_THRE) == 0);
    PUT32(UART_THR, x);
}

void PRINT(const char *s) {
    while (*s) uart_send(*s++);
}

void timer_setup() {
    DMTIMER_TCLR = 0;  // Asegúrate de parar el timer antes de configurar

    DMTIMER_TLDR = 0xFFFF0000;         // Valor que da 2 segundos (según manual)
    DMTIMER_TCRR = 0xFFFF0000;         // Valor inicial
    DMTIMER_IRQENABLE_SET = (1 << 1);  // Habilita interrupción por overflow

    // Sin prescaler, solo habilita start y autoreload
    DMTIMER_TCLR = (1 << 0) | (1 << 1);
}



void isr_timer2(){
    //rutina de servicio de interrupcion

    PRINT("******************Timer2 Interrupt******************\n");
    // Limpia la interrupción de overflow
    DMTIMER_IRQSTATUS |= (1 << 1);     // OVF_IT = bit 1
    // Señala fin de atención al INTC (EOI)
    DTIMER_IRQ_EOI = 1;

}
void os_main(void) {
    PRINT("Iniciando OS...\n");

    // Desmascarar interrupción del Timer2 (IRQ 68)
    INTC_MIR_CLEAR2 = (1 << (68 - 64));

    PRINT("Interrupcion Timer2 habilitada en INTC\n");

    // Configurar el Timer
    timer_setup();

    PRINT("Timer configurado\n");

    // Habilitar interrupciones globales
    asm volatile("cpsie i");
    PRINT("IRQ global habilitado\n");
    
    
        // Bucle infinito
        int i = 0;
        while (i<100) {
            i++;
        }
}