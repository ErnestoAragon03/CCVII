extern void enable_irq(void);
extern void PUT32(unsigned int, unsigned int);
extern unsigned int GET32(unsigned int);
#include "p1.h"
#include "p2.h"
#include "uart.h"
#include "timer.h"
#include "pcb.h"
#include "scheduler.h"


void os_main(void) {
    PRINT("Iniciando OS...\n");

    PRINT("Starting...\n");
    timer_init();
    enable_irq();

    PRINT("Initial TCRR: ");
    uart_hex(GET32(TCRR));
    PRINT("IRQ global habilitado\n");

    PRINT("Creando scheduler...\n");
    create_scheduler(); // Crear scheduler

    PRINT("Creando tabla de procesos...\n");
    create_table(); // Crear tabla de procesos


    PRINT("Creando procesos...\n");

    int pid1 = create_process(main_p1); // Crear proceso 1
    PRINT("Proceso 1 creado con PID: ");
    uart_decimal(pid1);
    PRINT("\n");
    if (pid1 < 0) {
        PRINT("Error al crear proceso 1\n");
    }

    int pid2 = create_process(main_p2); // Crear proceso 2
    PRINT("Proceso 2 creado con PID: ");
    uart_decimal(pid2);
    PRINT("\n");
    if (pid2 < 0) {
        PRINT("Error al crear proceso 2\n");
    }

   

    PRINT("Iniciando scheduler...\n");
    run_scheduler(); // Iniciar scheduler
}