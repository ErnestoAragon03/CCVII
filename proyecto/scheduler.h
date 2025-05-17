#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "pcb.h"
#include "waitingList.h"
#include "uart.h"
#include "utils.h"

extern PCB *current_process;
extern void irq_handler_end(unsigned int* sp);

void select_next_process(unsigned int sp);
int create_process(void (*function)(void));
void start_process(unsigned int pid);
int create_scheduler(void);
int run_scheduler(void);

#endif