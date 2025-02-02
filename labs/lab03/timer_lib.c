#include "timer_lib.h"
#include "os.h"
#include "stdio.h"

void start_timer() {
    timer_init();               // Inicializar el temporizador
    vic_enable_timer_irq();     // Habilitar la IRQ del Timer
    PRINT("Timer Started. Waiting for interrupts...\n");
}

void print_timer_value() {
    unsigned int timer_value = get_timer_value();
    PRINT("Timer Value: %d\n", timer_value); 
}

void handle_timer_interrupt() {
    timer_clear_interrupt();    // Limpiar la interrupción del temporizador
    PRINT("Timer Interrupt Triggered!\n");
}
