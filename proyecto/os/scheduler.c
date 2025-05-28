#include "scheduler.h"
#include "timer.h"

PCB *current_process = NULL;
unsigned int kernel_stacks[MAX_PROCESSES][1024];
unsigned int valor_LR=0;

unsigned int* initialize_stack(void (*func)(void), void* stack_base_addr) {
    unsigned int* sp = (unsigned int*)(stack_base_addr); // 4KB arriba
    sp+=1024;
    sp -= 14; // Espacio para simular contexto

    for (int i = 0; i < 13; i++) {
        sp[i] = 0x0;        // R0–R12 valores dummy
    }

    sp[13] = (unsigned int)func + 4; // lr a la funcion del proceso
    valor_LR = (unsigned int)func+4;
    return (void*)sp;
}


void* select_next_process(void* sp) {
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
            // Guardramos el stack pointer antiguo
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
                PRINT("Con SP:");
                uart_hex((unsigned int)current_process->stack_pointer);
                PRINT("\n");
                //si proceso no tiene stack inicializado (es nuevo)
                if(current_process->stack_pointer == current_process->stack){
                    PRINT("Inicializando stack del proceso con PID: ");
                    uart_decimal(current_process->pid);
                    PRINT("\n");
                    current_process->stack_pointer = initialize_stack(current_process->process_function, current_process->stack_pointer);


                    PRINT("Base del stack del proceso: ");
                    uart_hex((unsigned int)current_process->stack);
                    PRINT("\nStack pointer inicializado: ");
                    uart_hex((unsigned int)current_process->stack_pointer);
                    PRINT("\n");
                    PRINT("Valor de LR en stack: ");
                    uart_hex(valor_LR);
                    PRINT("\n");
                }
                
                PRINT("Soy la direccion de la funcion en select_next_process: ");
                uart_hex((unsigned int)current_process->process_function);
                PRINT("\n");
                PRINT("Llegue a end ");
                PRINT("SP: ");
                uart_hex((unsigned int)current_process->stack_pointer);
                return current_process->stack_pointer; // Retornar el stack pointer del proceso actual
                
            }
            else{
                PRINT("Error al seleccionar el siguiente proceso.\n");
            }
        } else {
            PRINT("No hay procesos en la lista de espera.\n");
        }
        return NULL;
}

int create_process(void (*function)(void)) {
    if(num_processes >= MAX_PROCESSES) return -1; // Límite alcanzado
    PCB *pcb = &process_table[num_processes];
    pcb->pid = num_processes;
    pcb->priority = 0; // Prioridad inicial
    pcb->state = CREATED;
    pcb->stack = &kernel_stacks[num_processes][0];
    pcb->stack_pointer = pcb->stack; // Inicializar el stack pointer al inicio del stack
    pcb->process_function = function;
    num_processes++;
    PRINT("Soy la direccion de la funcion en create_process: ");
    uart_hex((unsigned int)function);
    PRINT("\n");
    PRINT("Soy la direccion base del stack en create_process: ");
    uart_hex((unsigned int)pcb->stack);
    PRINT("\n");
    PRINT("Soy el valor del sp en create_process: ");
    uart_hex((unsigned int)pcb->stack_pointer);
    PRINT("\n");
    
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
    
    start_first_process(); 
    while(1);
}

