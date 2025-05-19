#include "pcb.h"

PCB process_table[MAX_PROCESSES];
int num_processes = 0;

void create_table(void) {
    num_processes = 0;
    for (int i = 0; i < MAX_PROCESSES; i++) {
        process_table[i].pid = -1;
        process_table[i].state = TERMINATED;
    }
}

// Direcciones físicas fijas de entrada y pila (coinciden con tu linker script)
#define STACK_BASE_P1  0x80018000
#define STACK_BASE_P2  0x80028000
#define ENTRY_P1       0x80010000
#define ENTRY_P2       0x80020000

int create_process(int pid) {
    if (pid >= MAX_PROCESSES) return -1;

    PCB *pcb = &process_table[pid];
    pcb->pid = pid;
    pcb->state = READY;
    pcb->priority = 0;

    unsigned int *stack_top;
    unsigned int entry_point;

    if (pid == 0) {
        stack_top = (unsigned int *)STACK_BASE_P1;
        entry_point = ENTRY_P1;
    } else if (pid == 1) {
        stack_top = (unsigned int *)STACK_BASE_P2;
        entry_point = ENTRY_P2;
    } else {
        return -1; // aún no soportamos más
    }

    // Simular contexto interrumpido: 13 regs + lr + cpsr + 2 extras = 17 slots
    stack_top -= 17;
    for (int i = 0; i < 17; i++) stack_top[i] = 0;

    // Posición 13 = LR (return address)
    stack_top[13] = entry_point; // 0x80010000 o 0x80020000

    // Posición 14 = SPSR (estado del procesador)
    stack_top[14] = 0x10; // modo usuario

    pcb->stack_pointer = (unsigned int)stack_top;

    num_processes++;
    return pid;
}
