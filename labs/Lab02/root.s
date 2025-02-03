.section .text
.global _start

// Dirección de la VIT
.global vector_table
vector_table:
    b irq_handler    // IRQ (Normal interruption)

_start:
    b reset

reset:
    ldr sp, =0x00020000   // Set up stack pointer (within RAM)
    bl main               // Call the main function

loop:
    b loop                // Infinite loop to prevent exit


/* ---- Handler de interrupciones ---- */
irq_handler:
    push {r0-r12, lr}

    ldr r0, =0x10140000     // Load base address of VIC
    ldr r1, [R0]            // Read VIC_IRQ_STATUS 
    cmp r1, #0
    beq timer_irq_handler   //No IRQ active

    tst r1, #(1 << 4)       //Verify if is a Timer interruption
    bne timer_irq_handler

irq_done:
    pop {r0-r12, lr}        //Restore register values
    subs pc, lr, #0         //Return from the interruption

timer_irq_handler:
    bl timer_isr            //Call the ISR from os.c
    ldr r0, =0x10140030     //VIC_VEC_ADDR 
    str r1, [R0]            //Clean interruption in VIC
    b irq_done              //Finish IRQ

