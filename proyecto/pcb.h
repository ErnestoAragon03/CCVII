#ifndef PCB_H
#define PCB_H

typedef enum {
    READY,
    RUNNING,
    BLOCKED,
    TERMINATED
} state_process;

typedef struct{
    unsigned int pid;
    unsigned int priority;
    unsigned int stack_pointer;
    state_process state;
    void (*process_function)(void);
} PCB;

extern int num_processes;
extern PCB process_table[];

void create_table(void);
int create_process(void (*function)(void));
void start_process(unsigned int pid);

#endif