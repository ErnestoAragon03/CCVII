#include "pcb.h"

// Esta debe ser tu variable global para saber cuál es el proceso actual
extern int current_pid;
extern PCB process_table[];

void save_context(unsigned int sp_value) {
    process_table[current_pid].stack_pointer = sp_value;
}

void restore_context(void) {
    unsigned int sp_value = process_table[current_pid].stack_pointer;
    asm volatile("mov sp, %0" :: "r"(sp_value));
}
