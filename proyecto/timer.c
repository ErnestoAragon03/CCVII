#include "timer.h"
#include "scheduler.h"
#include "uart.h"

extern void PUT32(unsigned int, unsigned int);
extern unsigned int GET32(unsigned int);

void timer_init(void) {
    PRINT("Step 1: Enable timer clock\n");
    PUT32(CM_PER_TIMER2_CLKCTRL, 0x2);

    PRINT("Step 2: Unmask IRQ 68\n");
    PUT32(INTC_MIR_CLEAR2, 1 << (68 - 64));
    PUT32(INTC_ILR_0 + 0x110, 0x0);  // INTC_ILR68: Priority 0, IRQ not FIQ

    PRINT("Step 3: Stop timer\n");
    PUT32(TCLR, 0);

    PRINT("Step 4: Clear interrupts\n");
    PUT32(TISR, 0x7);

    PRINT("Step 5: Set load value\n");//3s de timer
    PUT32(TLDR, 0xFE91CA00);

    PRINT("Step 6: Set counter\n");
    PUT32(TCRR, 0xFE91CA00);

    PRINT("Step 7: Enable overflow interrupt\n");
    PUT32(TIER, 0x2);

    PRINT("Step 8: Start timer with auto-reload\n");
    PUT32(TCLR, 0x3);

    PRINT("Timer initialized\n");
}

void* timer_irq_handler(void* sp) {
    PRINT("Stopping timer\n");
    PUT32(TCLR, 0x0);

    PUT32(TISR, 0x2);    
    PUT32(INTC_CONTROL, 0x1);

    PRINT("\nSP:\n");
    uart_hex((unsigned int)sp);
    PRINT("\n*****************************************************Tick*************************************************************\n");
    
    void* new_sp = select_next_process(sp);
    PRINT("\nSP del nuevo proceso:");
    uart_hex((unsigned int)new_sp);
    PRINT("\n");

    PRINT("Starting timer\n");
    PUT32(TCLR, 0x3);
    return new_sp;
}