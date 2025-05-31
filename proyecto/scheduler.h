#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "pcb.h"
#include "waitingList.h"
#include "uart.h"
#include "utils.h"

extern PCB *current_process;
extern void irq_handler_end(void* sp);
extern void start_first_process(void);

void* select_next_process(void* sp);
int create_process(void (*function)(void));
void start_process(unsigned int pid);
int create_scheduler(void);
int run_scheduler(void);

#endif