extern void enable_irq(void);
extern void PUT32(unsigned int, unsigned int);
extern unsigned int GET32(unsigned int);
#include "p1.h"
#include "p2.h"
#include "uart.h"
#include "timer.h"
#include "pcb.h"


void os_main(void) {
    PRINT("Iniciando OS...\n");

    PRINT("Starting...\n");
    timer_init();
    enable_irq();

    PRINT("Initial TCRR: ");
    uart_hex(GET32(TCRR));
    PRINT("IRQ global habilitado\n");
    
    PRINT("Tabla de procesos creada\n");
    PRINT("Creando procesos...\n");
    int pid = create_process(main_p1); // Crear proceso 1
    PRINT("Proceso 1 creado con PID: ");
    uart_decimal(pid);
    PRINT("\n");
    if (pid < 0) {
        PRINT("Error al crear proceso 1\n");
    }
    start_process(pid); // Iniciar proceso 1
}