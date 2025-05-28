#include "scheduler.h"

PCB *current_process = NULL;

void* initialize_stack(void (*func)(void), void* stack_base){
    unsigned int* sp = (unsigned int*)stack_base;

    // Simular el contexto esperado por el IRQ handler:
    // - push {r0-r12, lr}
    // - luego SP se pasa a timer_irq_handler y debe estar listo para:
    //   pop {r0-r12, lr}
    //   subs pc, lr, #4

    sp += 1024;  // Tope del stack (asumiendo 4KB por proceso)
    sp -= 14;    // r0-r12 + lr

    for (int i = 0; i < 13; i++) {
        sp[i] = 0x0;  // r0-r12
    }

    // IMPORTANTE: este lr debe apuntar a func + 4 para que `subs pc, lr, #4` salte a func
    sp[13] = (unsigned int)func + 4;

    return (void*)sp;
}


void* select_next_process(void* sp) {
    if (waiting_list == NULL) {
        PRINT("Error: La lista de espera no ha sido creada.\n");
    }

    PRINT("Waiting list size: ");
    uart_decimal(waiting_list->size);
    PRINT("\n");

    if (current_process != NULL) {
        PRINT("Guardando proceso actual con PID: ");
        uart_decimal(current_process->pid);
        PRINT("\n");

        current_process->stack_pointer = sp;
        current_process->state = READY;

        if (enqueue(current_process) == 0) {
            PRINT("Proceso con PID: ");
            uart_decimal(current_process->pid);
            PRINT(" agregado a la lista de espera.\n");
        } else {
            PRINT("❌ Error al agregar el proceso a la lista de espera.\n");
        }

        current_process = NULL;
    } else {
        PRINT("No hay proceso actual.\n");
    }

    if (waiting_list->size > 0) {
        current_process = dequeue();
        if (current_process != NULL) {
            current_process->state = RUNNING;

            PRINT("Ejecutando proceso con PID: ");
            uart_decimal(current_process->pid);
            PRINT("\n");
            PRINT("Con SP: ");
            uart_hex((unsigned int)current_process->stack_pointer);
            PRINT("\n");

            if (current_process->stack_pointer == current_process->stack) {
                PRINT("Inicializando stack del proceso con PID: ");
                uart_decimal(current_process->pid);
                PRINT("\n");

                current_process->stack_pointer = initialize_stack(
                    current_process->process_function,
                    current_process->stack_pointer
                );

                PRINT("Base del stack del proceso: ");
                uart_hex((unsigned int)current_process->stack);
                PRINT("\nStack pointer inicializado: ");
                uart_hex((unsigned int)current_process->stack_pointer);
                PRINT("\n");
            }

            PRINT("Llegue a end. SP: ");
            uart_hex((unsigned int)current_process->stack_pointer);
            PRINT("\n");

            return current_process->stack_pointer;
        } else {
            PRINT("Error al seleccionar el siguiente proceso.\n");
        }
    } else {
        PRINT("No hay procesos en la lista de espera.\n");
    }

    return sp; // Valor por defecto para evitar retorno vacío
}

int create_process(void (*function)(void)) {
    if(num_processes >= MAX_PROCESSES) return -1; // Límite alcanzado
    PCB *pcb = &process_table[num_processes];
    pcb->pid = num_processes;
    pcb->priority = 0; // Prioridad inicial
    pcb->state = CREATED;
    pcb->process_function = function;
    pcb->stack_pointer = pcb->stack; // Inicializar el stack pointer al inicio del stack
    num_processes++;
    return pcb->pid;
}

void start_process(unsigned int pid) {
    if(pid < 0 || pid >= num_processes) return; // PID inválido
    PCB *pcb = &process_table[pid];
    pcb->state = READY;
    enqueue(pcb);
}

int create_scheduler(void) {
    WaitingList* waiting_list = createWaitingList();

    if (waiting_list == NULL) {
        return -1; // Error al crear la lista de espera
    }

    current_process = NULL; // Inicializar el proceso actual

    return 0; // Éxito
}

int run_scheduler(void) {
    if (waiting_list == NULL) {
        return -1; // Error al recuperar la lista de espera
    }

    start_first_process(); 
    while(1);
}
