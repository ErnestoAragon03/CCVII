#define UART0_BASE 0x44E09000
#define UART_THR (UART0_BASE + 0x00)
#define UART_LSR (UART0_BASE + 0x14)
#define UART_LSR_THRE 0x20

typedef unsigned int uint;


extern void PUT32(uint addr, uint value);
extern uint GET32(uint addr);


void uart_send(char c) {
    while ((GET32(UART_LSR) & UART_LSR_THRE) == 0);
    PUT32(UART_THR, c);
}

void uart_puts(const char* s) {
    while (*s) uart_send(*s++);
}


void delay(volatile uint count) {
    while (count--) {
        asm volatile("nop");
    }
}

void child_process() {
    uart_puts("[Child] Proceso creado (simulado PID: 1001)\n");

    uart_puts("[Child] Estado: RUNNING\n");
    delay(5000000);

    uart_puts("[Child] Estado: SLEEPING (simulado)\n");
    delay(10000000);

    uart_puts("[Child] Estado: STOPPED (simulado, esperando señal)\n");
    delay(15000000);  

    uart_puts("[Child] Estado: RESUMED -> RUNNING\n");
    delay(5000000);

    uart_puts("[Child] Finalizando proceso.\n");
}


void parent_process() {
    uart_puts("[Parent] Observando proceso hijo (PID: 1001)\n");
    delay(2000000);

    uart_puts("[Parent] Estado del hijo: STOPPED\n");
    delay(1000000);

    uart_puts("[Parent] Enviando señal SIGCONT al hijo\n");
    delay(1000000);
}

int main(void) {
    uart_puts("🔧 Simulación de Estados de Proceso (Bare-metal)\n");

    
    child_process();

    
    parent_process();

    uart_puts("[Parent] Proceso hijo ha terminado\n");
    uart_puts("✅ Simulación completa.\n");

    while (1);
    return 0;
}
