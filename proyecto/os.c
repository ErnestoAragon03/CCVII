
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
#define INTC_ILR68             (*(volatile unsigned int *)(INTC_BASE + 0x100 + (68 * 4)))// prioridad del Timer2 (IRQ 68)
#define INTC_SIR_IRQ           (*(volatile unsigned int *)(INTC_BASE + 0x40))//contiene el numero de la interrupcion que ha sido activada
#define INTC_MIR_CLEAR2         (*(volatile unsigned int *)(INTC_BASE + 0xC8)) //MIR_CLEAR2 para liberar la interrupcion 68 (Timer2)

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
void uart_hex(unsigned int value) {
    for (int i = 7; i >= 0; i--) {
        unsigned int nibble = (value >> (i * 4)) & 0xF;
        uart_send(nibble > 9 ? ('A' + nibble - 10) : ('0' + nibble));
    }
}


void PRINT(const char *s) {
    while (*s) uart_send(*s++);
}

void timer_setup() {
    PRINT("Configurando Timer2...\n");
    DMTIMER_TCLR = 0;  // Asegúrate de parar el timer antes de configurar

    // Valor que produce una interrupción cada 10 segundos
    DMTIMER_TLDR = 0xF1F9A100;
    DMTIMER_TCRR = 0xF1F9A100;         // Valor inicial
    DMTIMER_IRQENABLE_SET = (1 << 1);  // Habilita interrupción por overflow

    // Sin prescaler, solo habilita start y autoreload
    DMTIMER_TCLR = (1 << 0) | (1 << 1);
    PRINT("Timer2 configurado\n");
}



void isr_timer2(){
    //rutina de servicio de interrupcion
    PRINT("******************Timer2 Interrupt******************\n");
      // Agregar un pequeño retardo

    // Limpia la interrupción de overflow
    DMTIMER_IRQSTATUS = (1 << 1);     // OVF_IT = bit 1

    *(volatile unsigned int*)0x48200048 = 0x1;  // NEWIRQAGR, fin de atencion IRQ

    //asegurarse que se commpleto la escritura
    __asm__ __volatile__("dsb");
    // Señala fin de atención al INTC (EOI)
    DTIMER_IRQ_EOI = 1;

    INTC_CONTROL = 0x1;

}
void os_main(void) {
    PRINT("Iniciando OS...\n");

    INTC_ILR68 = 0x0;  // Nivel de prioridad más alto (0x0), tipo IRQ
    // Desmascarar interrupción del Timer2 (IRQ 68)
    INTC_MIR_CLEAR2 = (1 << (68 - 64));

    PRINT("Interrupcion Timer2 habilitada en INTC\n");

    

    // Habilitar interrupciones globales
    asm volatile("cpsie i");
    // Configurar el Timer
    timer_setup();
    PRINT("IRQ global habilitado\n");

}