#include "scheduler.h"
#include "pcb.h"
#include "context.h"

int current_pid = -1;
extern int num_processes;
extern PCB process_table[];

void create_scheduler() {
    current_pid = -1;
}

// Algoritmo Round-Robin simple
void select_next_process() {
    current_pid++;
    if (current_pid >= num_processes) {
        current_pid = 0;
    }
}

void run_scheduler() {
    current_pid = 0;
    unsigned int sp_value = process_table[current_pid].stack_pointer;

    asm volatile(
        "mov sp, %0\n"                 // cargar SP del proceso
        "ldmia sp, {r0-r12, lr}\n"     // restaurar registros generales y lr
        "ldr r1, [sp, #64]\n"          // leer SPSR simulado del stack
        "msr spsr_cxsf, r1\n"          // cargar SPSR
        "add sp, sp, #68\n"            // limpiar stack simulado
        "subs pc, lr, #4\n"            // salto final a proceso (modo usuario)
        :: "r"(sp_value)
    );
}

