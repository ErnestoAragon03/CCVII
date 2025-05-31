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