#include "scheduler.h"
#include "pcb.h"
#include "context.h"

int current_pid = -1;  // ID del proceso actual
extern int num_processes;
extern PCB process_table[];

// Selecciona el siguiente proceso de forma Round-Robin (sin división)
void select_next_process() {
    current_pid++;
    if (current_pid >= num_processes) {
        current_pid = 0;
    }
}

// Inicializa el scheduler
void create_scheduler() {
    current_pid = -1;
}

// Corre el scheduler: inicia el primer proceso
void run_scheduler() {
    current_pid = 0;           // Comienza con el primer proceso
    restore_context();         // Carga el stack del proceso 0
}
