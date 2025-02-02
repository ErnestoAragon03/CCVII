#include "timer_lib.h"

int main() {
    PRINT("Starting Timer Test...\n");

    start_timer();  // Usar la función de la librería

    while (1) {
        print_timer_value();  // Usar la función de la librería
        delay(500000);         // Pausa para no saturar UART
    }

    return 0;
}
