
extern void enable_irq(void);
extern void PUT32(unsigned int, unsigned int);
extern unsigned int GET32(unsigned int);
extern void os_main(void);
extern void app_main(void);
#define UART0_BASE     0x44E09000
#define UART_THR       (UART0_BASE + 0x00)
#define UART_LSR       (UART0_BASE + 0x14)
#define UART_LSR_THRE  0x20

#define DMTIMER2_BASE    0x48040000
#define TCLR             (DMTIMER2_BASE + 0x38)
#define TCRR             (DMTIMER2_BASE + 0x3C)
#define TISR             (DMTIMER2_BASE + 0x28)
#define TIER             (DMTIMER2_BASE + 0x2C)
#define TLDR             (DMTIMER2_BASE + 0x40)

#define INTCPS_BASE      0x48200000
#define INTC_MIR_CLEAR2  (INTCPS_BASE + 0xC8)
#define INTC_CONTROL     (INTCPS_BASE + 0x48)

#define CM_PER_BASE      0x44E00000
#define CM_PER_TIMER2_CLKCTRL (CM_PER_BASE + 0x80)

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

void timer_init(void) {
    PRINT("Step 1: Enable timer clock\n");
    PUT32(CM_PER_TIMER2_CLKCTRL, 0x2);

    PRINT("Step 2: Unmask IRQ 68\n");
    PUT32(INTC_MIR_CLEAR2, 1 << (68 - 64));
    PUT32(INTCPS_BASE + 0x110, 0x0);  // INTC_ILR68: Priority 0, IRQ not FIQ

    PRINT("Step 3: Stop timer\n");
    PUT32(TCLR, 0);

    PRINT("Step 4: Clear interrupts\n");
    PUT32(TISR, 0x7);

    PRINT("Step 5: Set load value\n");
    PUT32(TLDR, 0xFE91CA00);

    PRINT("Step 6: Set counter\n");
    PUT32(TCRR, 0xFE91CA00);

    PRINT("Step 7: Enable overflow interrupt\n");
    PUT32(TIER, 0x2);

    PRINT("Step 8: Start timer with auto-reload\n");
    PUT32(TCLR, 0x3);

    PRINT("Timer initialized\n");
}

void timer_irq_handler(void) {
    PUT32(TISR, 0x2);
    
    PUT32(INTC_CONTROL, 0x1);
    PRINT("*****************************************************Tick*************************************************************\n");
    int i=0;
    while(i<1000) {
        i++;
    }
}

void os_main(void) {
    PRINT("Iniciando OS...\n");

    PRINT("Starting...\n");
    timer_init();
    enable_irq();

    PRINT("Initial TCRR: ");
    uart_hex(GET32(TCRR));
    PRINT("IRQ global habilitado\n");

    app_main(); // Llama a la función principal de la aplicación
}