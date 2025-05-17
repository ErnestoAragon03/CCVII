#include "scheduler.h"

PCB *current_process = NULL;

void select_next_process(unsigned int* sp) {
    // Implementación del planificador
    // Aquí se selecciona el siguiente proceso a ejecutar
    // y se realiza el cambio de contexto si es necesario.
    //PRINT("Seleccionando siguiente proceso...\n");

    //Quitar el proceso actual de ejecución (Si es que hay uno)
    if (current_process != NULL) {
        current_process->state = READY;
        //Meterlo a la lista de espera
        current_process->stack_pointer = sp;
        enqueue(current_process);
        current_process = NULL;
    }

    //Seleccionar el siguiente proceso
    if (waiting_list->size > 0) {
        current_process = dequeue();
        if (current_process != NULL) {
            current_process->state = RUNNING;
            // Aquí se puede implementar el cambio de contexto
            // y la ejecución del proceso seleccionado.
            PRINT("Ejecutando proceso con PID: ");
            uart_decimal(current_process->pid);
            PRINT("\n");
            if(sp != 0){
                PRINT("Llegue a end");
                irq_handler_end(current_process->stack_pointer);
            }
            else{
                current_process->process_function();
            }
            
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
    pcb->stack_pointer = 0;
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

int run_scheduler(void) {
    // Iniciar el planificador
    // Aquí se puede implementar la lógica para iniciar el planificador
    // y gestionar los procesos en la lista de espera.
    if (waiting_list == NULL) {
        return -1; // Error al recuperar la lista de espera
    }
    while(1){
        select_next_process(0);
    }    
}