extern void enable_irq(void);
extern void PUT32(unsigned int, unsigned int);
extern unsigned int GET32(unsigned int);
#include "p1.h"
#include "p2.h"
#include "uart.h"
#include "timer.h"


void os_main(void) {
    PRINT("Iniciando OS...\n");

    PRINT("Starting...\n");
    timer_init();
    enable_irq();

    PRINT("Initial TCRR: ");
    uart_hex(GET32(TCRR));
    PRINT("IRQ global habilitado\n");

    main_p1(); // Llama a la función principal del programa 1
}