#include "pcb.h"
#include "uart.h"

#define MAX_PROCESSES 10
PCB process_table[MAX_PROCESSES];
int num_processes = 0;

void create_table(void) {
    num_processes = 0; // Inicializar el número de procesos
    for(int i = 0; i < MAX_PROCESSES; i++) {
        process_table[i].pid = -1; // PID no asignado
        process_table[i].state = TERMINATED; // Estado inicial
    }
}

int create_process(void (*function)(void)) {
    if(num_processes >= MAX_PROCESSES) return -1; // Límite alcanzado
    PCB *pcb = &process_table[num_processes];
    pcb->pid = num_processes;
    pcb->priority = 0; // Prioridad inicial
    pcb->state = READY;
    pcb->process_function = function;
    num_processes++;
    return pcb->pid;
}

void start_process(unsigned int pid) {
    if(pid < 0 || pid >= num_processes) return; // PID inválido
    PCB *pcb = &process_table[pid];
    pcb->state = RUNNING;
    pcb->process_function();
    pcb->state = TERMINATED;
}