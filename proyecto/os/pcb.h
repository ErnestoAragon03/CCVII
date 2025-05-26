#ifndef PCB_H
#define PCB_H
#define STACK_SIZE 1024

typedef enum {
    CREATED,
    READY,
    RUNNING,
    BLOCKED,
    TERMINATED
} state_process;

typedef struct{
    unsigned int pid;
    unsigned int cpu_usage;
    unsigned int priority;
    unsigned int stack_pointer;
    state_process state;
    void (*process_function)(void);
} PCB;

#define MAX_PROCESSES 4

extern int num_processes;
extern PCB process_table[];

void create_table(void);


#endif