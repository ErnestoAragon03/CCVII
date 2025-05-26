#ifndef PCB_H
#define PCB_H

typedef enum {
    CREATED,
    READY,
    RUNNING,
    BLOCKED,
    TERMINATED
} state_process;

typedef struct{
    unsigned int pid;
    unsigned int priority;
    void* stack_pointer;
    state_process state;
    void (*process_function)(void);
} PCB;

#define MAX_PROCESSES 10

extern int num_processes;
extern PCB process_table[];

void create_table(void);


#endif