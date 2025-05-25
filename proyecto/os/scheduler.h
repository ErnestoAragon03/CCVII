#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "pcb.h"
#include "waitingList.h"
#include "uart.h"
#include "utils.h"

extern PCB *current_process;

void select_next_process(void);
int create_process(void (*function)(void));
void start_process(unsigned int pid);
int create_scheduler(void);
int run_scheduler(void);
void save_context(PCB *p);
void restore_context(PCB *p);
void process_exit(void) ;

#endif