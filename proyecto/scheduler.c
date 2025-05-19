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

// ⚠️ Lanzamiento inicial del primer proceso directamente desde el OS
void run_scheduler() {
    current_pid = 0;
    unsigned int sp_value = process_table[current_pid].stack_pointer;

    asm volatile(
        "mov sp, %0\n"                 // cargar stack del proceso
        "ldmfd sp!, {r0-r12, lr}\n"    // restaurar registros
        "ldmfd sp!, {r1}\n"            // r1 = cpsr simulado
        "msr spsr_cxsf, r1\n"          // cargar spsr
        "movs pc, lr\n"                // salto al proceso con modo usuario
        :: "r"(sp_value)
    );
}
