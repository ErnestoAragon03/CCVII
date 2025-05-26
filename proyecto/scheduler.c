#include "scheduler.h"

PCB *current_process = NULL;

void select_next_process(void* sp) {
        // Implementación del planificador
        // Aquí se selecciona el siguiente proceso a ejecutar
        // y se realiza el cambio de contexto si es necesario.

        //Quitar el proceso actual de ejecución (Si es que hay uno)
        if(waiting_list == NULL) {
            PRINT("Error: La lista de espera no ha sido creada.\n");
        }
        PRINT("Waiting list size: ");
        uart_decimal(waiting_list->size);
        PRINT("\n");
        if (current_process != NULL) {
            PRINT("Guardando proceso actual con PID: ");
            uart_decimal(current_process->pid);
            PRINT("\n");
            // Guardar el nuevo stack pointer
            current_process->stack_pointer = sp;
            // Cambiar el estado del proceso actual a READY
            current_process->state = READY;
            //Meterlo a la lista de espera
            if(enqueue(current_process) == 0 ){
                PRINT("Proceso con PID: ");
                uart_decimal(current_process->pid);
                PRINT(" agregado a la lista de espera.\n");
            } else {
                PRINT("Error al agregar el proceso a la lista de espera.\n");
            }
            current_process = NULL;
        }
        else {
            PRINT("No hay proceso actual.\n");
        }

        //Seleccionar el siguiente proceso
        if (waiting_list->size > 0) {
            current_process = dequeue();
            if (current_process != NULL) {
                current_process->state = RUNNING;
                PRINT("Ejecutando proceso con PID: ");
                uart_decimal(current_process->pid);
                PRINT("\n");
                sp = current_process->stack_pointer;

                PRINT("Con SP:");
                uart_hex((unsigned int)sp);
                PRINT("\n");
                if(sp != 0){
                    PRINT("Llegue a end ");
                    PRINT("SP: ");
                    uart_hex((unsigned int)sp);
                    irq_handler_end(sp);
                }
                else{
                    current_process->process_function();
                    PRINT("Regreso\n");
                }
                
            }
            else{
                PRINT("Error al seleccionar el siguiente proceso.\n");
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