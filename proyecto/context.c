#include "pcb.h"
#include "uart.h"


// Esta debe ser tu variable global para saber cuál es el proceso actual
extern int current_pid;
extern PCB process_table[];


    void validate_stack(int pid) {
        unsigned int sp = process_table[pid].stack_pointer;

        if (pid == 0 && (sp < 0x80018000 || sp > 0x80018FFF)) {
            PRINT("❌ [P1] SP FUERA DE RANGO: ");
            uart_hex(sp);
            PRINT("\n");
        } else if (pid == 1 && (sp < 0x80028000 || sp > 0x80028FFF)) {
            PRINT("❌ [P2] SP FUERA DE RANGO: ");
            uart_hex(sp);
            PRINT("\n");
        } else {
            PRINT("✅ SP de PID ");
            uart_decimal(pid);
            PRINT(" dentro del rango válido: ");
            uart_hex(sp);
            PRINT("\n");
        }
    }
void save_context(unsigned int sp_value) {
        PRINT("💾 save_context PID=");
        uart_decimal(current_pid);
        PRINT(" SP=0x");
        uart_hex(sp_value);
        PRINT("\n");

        process_table[current_pid].stack_pointer = sp_value;
    }


    //*
    unsigned int restore_context(void) {
        unsigned int sp = process_table[current_pid].stack_pointer;
    
        validate_stack(current_pid); 
    
        PRINT("♻️ restore_context PID=");
        uart_decimal(current_pid);
        PRINT(" SP=0x");
        uart_hex(sp);
        PRINT("\n");
    
        return sp;
    }
    
