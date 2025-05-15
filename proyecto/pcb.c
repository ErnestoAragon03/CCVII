#include "pcb.h"

PCB process_table[MAX_PROCESSES];
int num_processes = 0;

void create_table(void) {
    num_processes = 0; // Inicializar el número de procesos
    for(int i = 0; i < MAX_PROCESSES; i++) {
        process_table[i].pid = -1; // PID no asignado
        process_table[i].state = TERMINATED; // Estado inicial
    }
}

// Define las direcciones de pila según tu mapa de memoria
#define STACK_SIZE     0x1000

#define STACK_BASE_P1  0x82030000  // última parte del RAM
#define STACK_BASE_P2  0x82038000  // separadas 32KB por stack (puedes ajustar)
#define STACK_BASE_P3  0x82020000  // opcional para más procesos


int create_process(void (*func)(void)) {
    if (num_processes >= MAX_PROCESSES) return -1;

    PCB *pcb = &process_table[num_processes];
    pcb->pid = num_processes;
    pcb->state = READY;
    pcb->priority = 0;
    pcb->process_function = func;

    unsigned int *stack_top;
    if (num_processes == 0)
        stack_top = (unsigned int *)STACK_BASE_P1;
    else if (num_processes == 1)
        stack_top = (unsigned int *)STACK_BASE_P2;
    else
        stack_top = (unsigned int *)STACK_BASE_P3;

    // Reservar espacio para contexto: r0–r12 (13) + lr + cpsr = 15 registros
    stack_top -= 15;

    // Limpiar la pila
    for (int i = 0; i < 15; i++) stack_top[i] = 0;

    // LR simulado (posición 13): dirección del proceso
    stack_top[13] = (unsigned int)func;

    // CPSR simulado: modo usuario (0x10)
    stack_top[14] = 0x10;

    // Guardar puntero de pila en el PCB
    pcb->stack_pointer = (unsigned int)stack_top;

    return num_processes++;
}
