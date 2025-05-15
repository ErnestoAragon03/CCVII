#ifndef SCHEDULER_H
#define SCHEDULER_H

void create_scheduler(void);
void select_next_process(void);
void run_scheduler(void);

extern int current_pid;

#endif
