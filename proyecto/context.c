#include "pcb.h"

// Esta debe ser tu variable global para saber cuál es el proceso actual
extern int current_pid;
extern PCB process_table[];

void save_context(unsigned int sp_value) {
    PRINT("💾 save_context PID=");
    uart_decimal(current_pid);
    PRINT(" SP=0x");
    uart_hex(sp_value);
    PRINT("\n");

    process_table[current_pid].stack_pointer = sp_value;
}

unsigned int restore_context(void) {
    unsigned int sp = process_table[current_pid].stack_pointer;

    PRINT("♻️ restore_context PID=");
    uart_decimal(current_pid);
    PRINT(" SP=0x");
    uart_hex(sp);
    PRINT("\n");

    return sp;
}
