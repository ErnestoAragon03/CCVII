/* hello.c */
extern void PUT32(unsigned int, unsigned int);
extern unsigned int GET32(unsigned int);
extern void enable_irq(void);

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

unsigned int seed = 12345;
unsigned int rand(void) {
    seed = (seed * 1103515245 + 12345) & 0x7fffffff;
    return seed;
}

void uart_send(unsigned char x) {
    while ((GET32(UART_LSR) & UART_LSR_THRE) == 0);
    PUT32(UART_THR, x);
}

void uart_puts(const char *s) {
    while (*s) {
        uart_send(*s++);
    }
}

void uart_putnum(unsigned int num) {
    char buf[10];
    int i = 0;
    do {
        buf[i++] = (num % 10) + '0';
        num /= 10;
    } while (num > 0);
    while (i > 0) {
        uart_send(buf[--i]);
    }
    uart_send('\n');
}

void uart_puthex(unsigned int num) {
    const char *hex = "0123456789ABCDEF";
    uart_puts("0x");
    for (int i = 28; i >= 0; i -= 4) {
        uart_send(hex[(num >> i) & 0xF]);
    }
    uart_send('\n');
}

void timer_init(void) {
    uart_puts("Step 1: Enable timer clock\n");
    PUT32(CM_PER_TIMER2_CLKCTRL, 0x2);

    uart_puts("Step 2: Unmask IRQ 68\n");
    PUT32(INTC_MIR_CLEAR2, 1 << (68 - 64));
    PUT32(INTCPS_BASE + 0x110, 0x0);  // INTC_ILR68: Priority 0, IRQ not FIQ

    uart_puts("Step 3: Stop timer\n");
    PUT32(TCLR, 0);

    uart_puts("Step 4: Clear interrupts\n");
    PUT32(TISR, 0x7);

    uart_puts("Step 5: Set load value\n");
    PUT32(TLDR, 0xFE91CA00);

    uart_puts("Step 6: Set counter\n");
    PUT32(TCRR, 0xFE91CA00);

    uart_puts("Step 7: Enable overflow interrupt\n");
    PUT32(TIER, 0x2);

    uart_puts("Step 8: Start timer with auto-reload\n");
    PUT32(TCLR, 0x3);

    uart_puts("Timer initialized\n");
}

void timer_irq_handler(void) {
    PUT32(TISR, 0x2);
    PUT32(INTC_CONTROL, 0x1);
    uart_puts("Tick\n");
}

int main(void) {

    uart_puts("Starting...\n");
    timer_init();
    enable_irq();

    uart_puts("Initial TCRR: ");
    uart_puthex(GET32(TCRR));

    while (1) {
        uart_putnum(rand() % 1000);
        for (volatile int i = 0; i < 100000000; i++);
    }
    return 0;
}
