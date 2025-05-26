#include "scheduler.h"
#include "timer.h"
#include "pcb.h"
PCB *current_process = NULL;
unsigned int process_stacks[MAX_PROCESSES][STACK_SIZE];

void select_next_process(void) {
    // Implementación del planificador
    // Aquí se selecciona el siguiente proceso a ejecutar
    // y se realiza el cambio de contexto si es necesario.
    //PRINT("Seleccionando siguiente proceso...\n");

    //Quitar el proceso actual de ejecución (Si es que hay uno)
    if (current_process != NULL) {
        current_process->state = READY;//proceso actual pasa a estado READY
        //Meterlo a la lista de espera
        enqueue(current_process);//se agrega a la cola de espera
        current_process = NULL;//se libera espacio de ejecucion 
    }

    //Seleccionar el siguiente proceso
    if (waiting_list->size > 0) {//si existen procesos en la lista de espera
        current_process = dequeue();//seleccionar el siguiente proceso
        if (current_process != NULL) {
            current_process->state = RUNNING;
            current_process->cpu_usage++; // Incrementar uso de CPU
            // Aquí se puede implementar el cambio de contexto
            // y la ejecución del proceso seleccionado.
            PRINT("Ejecutando proceso con PID: ");
            uart_decimal(current_process->pid);
            PRINT("\n");
        }
    } else {
        PRINT("No hay procesos en la lista de espera.\n");
    }
}

int create_process(void (*function)(void)) {
    if(num_processes >= MAX_PROCESSES) return -1; // Límite alcanzado
    PCB *pcb = &process_table[num_processes];
    pcb->pid = num_processes;
    pcb->priority = 0; // Prioridad inicial
    pcb->state = CREATED;
    pcb->process_function = function;
    pcb->cpu_usage = 0; // Uso de CPU inicial

    unsigned int *stack_top = &process_stacks[num_processes][STACK_SIZE - 1];

    *(--stack_top) = 0x60000013;  // CPSR (modo supervisor o usuario)
    *(--stack_top) = (unsigned int)function; // PC (inicio del proceso)
    *(--stack_top) = 0x0; // LR
    *(--stack_top) = 0x0; // R12
    *(--stack_top) = 0x0; // R11
    *(--stack_top) = 0x0; // R10
    *(--stack_top) = 0x0; // R9
    *(--stack_top) = 0x0; // R8
    *(--stack_top) = 0x0; // R7
    *(--stack_top) = 0x0; // R6
    *(--stack_top) = 0x0; // R5
    *(--stack_top) = 0x0; // R4
    *(--stack_top) = 0x0; // R3
    *(--stack_top) = 0x0; // R2
    *(--stack_top) = 0x0; // R1
    *(--stack_top) = 0x0; // R0

    pcb->stack_pointer = (unsigned int)stack_top;
    num_processes++;
    return pcb->pid;
}


void start_process(unsigned int pid) {
    if(pid < 0 || pid >= num_processes) return; // PID inválido
    PCB *pcb = &process_table[pid];
    pcb->state = READY;
    // Meterlo a la Ready List
    enqueue(pcb);
}

int create_scheduler(void) {
    // Iniciar el planificador
    // Aquí se puede implementar la lógica para iniciar el planificador
    // y gestionar los procesos en la lista de espera.
    WaitingList* waiting_list = createWaitingList();

    if (waiting_list == NULL) {
        return -1; // Error al crear la lista de espera
    }

    current_process = NULL; // Inicializar el proceso actual

    return 0; // Éxito

}

int  run_scheduler(void) {
    // Iniciar el planificador
    // Aquí se puede implementar la lógica para iniciar el planificador
    // y gestionar los procesos en la lista de espera.
    if (waiting_list == NULL) {
        return -1; // Error al recuperar la lista de espera
    }
    while(1){
        asm volatile ("wfi"); // Wait for interrupt
    }    
}