.section .text
.global _start
.extern timer_isr

// Dirección de la VIT
.global vector_table
vector_table:
    bl irq_handler    // IRQ (Normal interruption)

_start:
    b reset

reset:
    msr cpsr_c, vector_table       
    ldr sp, =0x00020000   // Set up stack pointer (within RAM)
    bl main               // Call the main function

loop:
    b loop                // Infinite loop to prevent exit


/* ---- Handler de interrupciones ---- */
irq_handler:
    push {r0-r12, lr}

    ldr r0, =0x10140000     // Load base address of VIC
    ldr r1, [r0]            // Read VIC_IRQ_STATUS 
    cmp r1, #0
    beq irq_done            // No IRQ active, exit

    tst r1, #(1 << 4)       // Check if Timer interrupt is active
    beq timer_irq_handler            // If not, exit

timer_irq_handler:
    bl timer_isr            // Call the ISR from os.c
    ldr r0, =0x10140030     // VIC_VEC_ADDR
    mov r1, #0              // Any value to acknowledge the IRQ
    str r1, [r0]            // Clear the interrupt in VIC

irq_done:
    pop {r0-r12, lr}        // Restore register values
    subs pc, lr, #0         // Return from the interruption