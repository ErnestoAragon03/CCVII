#include "os.h"

#define UART0_BASE 0x101f1000

#define UART_DR      0x00  // Data Register escriben/leen los datos
#define UART_FR      0x18  // Flag Register si el buffer de transmisión está lleno o el de recepción está vacío
#define UART_FR_TXFF 0x20  // Transmit FIFO Full  si el buffer de transmisión está lleno.
#define UART_FR_RXFE 0x10  // Receive FIFO Empty si el buffer de recepción está vacío.

//Direcciones de los Timers (Para Versatile Application Baseboard for ARM926EJ-S)
#define TIMER_0 0x101E2000
#define TIMER_0_LOAD (*(volatile int *) (TIMER_0 + 0x00))
#define TIMER_0_CTRL (*(volatile int *) (TIMER_0 + 0x08))
#define TIMER_0_CLR (*(volatile int *) (TIMER_0 + 0x0c))

//Dirección Base del Vector Interrupt Controller (VIC)
#define VIC_BASE 0x10140000
#define PIC_INT_STATUS (*(volatile int *) (VIC_BASE + 0x00))
#define PIC_INT_ENABLE (*(volatile int *) (VIC_BASE + 0x10))
#define PIC_INT_SELECT (*(volatile int *) (VIC_BASE + 0x0c))
#define PIC_VECT_ADDR (*(volatile int *) (VIC_BASE + 0x30))
 
volatile unsigned int *const UART0 = (unsigned int *)UART0_BASE; //permite acceder a los registros

// Enviar un carácter por UART
void uart_putc(char c) {
    while (UART0[UART_FR / 4] & UART_FR_TXFF);
    UART0[UART_DR / 4] = c;
}


// Recibir un carácter por UART
char uart_getc() {
    while (UART0[UART_FR / 4] & UART_FR_RXFE);
    return (char)(UART0[UART_DR / 4] & 0xFF);
}

// Enviar una cadena por UART
void uart_puts(const char *s) {
    while (*s) {
        uart_putc(*s++);
    }
}

// Termina si recibe  \n y \r
void uart_gets_input(char *buffer, int max_length) {
    int i = 0;
    char c;
    while (i < max_length - 1) {
        c = uart_getc();
        if (c == '\n' || c == '\r') {
            uart_putc('\n');
            break;
        }
        uart_putc(c);
        buffer[i++] = c;
    }
    buffer[i] = '\0';
}

// Inicialización del timer
void timer_setup(int miliseconds){
    /*Handles a timer interrupt made up by the user
    Clock freq = 32KHz
    Argumentos:
        miliseconds: Time that the timer should wait
    */
    //Escalar Valor de miliseconds (32*10^3)
    miliseconds *= 32000;
    //Cargar Valor en Timer
    TIMER_0_LOAD = miliseconds;
    //Limpiar bandera de interrupción
    TIMER_0_CLR = 1;
    //Configurar Timer
    TIMER_0_CTRL = (1<<7) | (1<<6) | (1<<5) | (1<<1);     //TimerEn, TimerMode, IntEnable, TimerSize

}

//Configure VIC for Interrupt Handling
void enable_timer_irq(){
    //Habilitar IRQ #4 (Timer IRQ) en el VIC
    PIC_INT_ENABLE |= (1<<4);
    //Asegurarse que la interrupción sea IRQ y no FIQ
    PIC_INT_SELECT &= ~(1<<4);
}

void timer_isr(){
    //Limpiar Banderas de interrupción
    TIMER_0_CLR = 1;
    uart_puts("¡Interrupción de Timer activada!");
}