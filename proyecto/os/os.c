extern void enable_irq(void);
extern void PUT32(unsigned int, unsigned int);
extern unsigned int GET32(unsigned int);

#include "uart.h"
#include "timer.h"
#include "pcb.h"
#include "scheduler.h"

// Direcciones fijas de los binarios de usuario
#define PROC1_ADDR ((void (*)(void))0x80010000)
#define PROC2_ADDR ((void (*)(void))0x80020000)

void os_main(void) {
    PRINT("Iniciando OS...\n");

    PRINT("Inicializando timer...\n");
    timer_init();
    enable_irq();

    PRINT("Valor inicial de TCRR: ");
    uart_hex(GET32(TCRR));
    PRINT("\nIRQ global habilitado\n");

    PRINT("Creando scheduler...\n");
    create_scheduler();

    PRINT("Creando tabla de procesos...\n");
    create_table();

    PRINT("Creando procesos...\n");

    int pid1 = create_process(PROC1_ADDR);
    PRINT("Proceso 1 creado con PID: ");
    uart_decimal(pid1);
    PRINT("\n");
    if (pid1 < 0) {
        PRINT("Error al crear proceso 1\n");
    }

    int pid2 = create_process(PROC2_ADDR);
    PRINT("Proceso 2 creado con PID: ");
    uart_decimal(pid2);
    PRINT("\n");
    if (pid2 < 0) {
        PRINT("Error al crear proceso 2\n");
    }

    start_process(pid1);
    start_process(pid2);

    PRINT("Iniciando scheduler...\n");
    run_scheduler();
}
