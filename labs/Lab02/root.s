.section .vectors, "x"
.global _vectors

_vectors:
    LDR PC, =irq_handler @ IRQ (Normal interruption)

.section .text
.global _start

_start:
b reset

reset:
ldr sp, =0x00020000   @ Set up stack pointer (within RAM)
bl main               @ Call the main function
loop:
b loop                @ Infinite loop to prevent exit


.extern timer_irq_service   @ Asegurar que el linker reconozca la función

irq_handler:
    SUB LR, LR, #4  @ Adjust to return
    STMFD SP!, {R0-R3, LR} @Save registers in the stack

    LDR R0, =0x10140000 @Load base address of VIC
    LDR R1, [R0]        @Read VIC_IRQ_STATUS 
    CMP R1, #0
    BEQ irq_done        @No IRQ active

    TST R1, #(1 << 4)   @Verify if is a Timer interruption
    BNE timer_irq_handler

irq_done:
    LDMFD SP!, {R0-R3, LR}  @Restore register values
    ADDS PC, SP, #0         @Return from the interruption

timer_irq_handler:
    BL timer_isr        @Call the ISR from os.c
    LDR R0, =0x10140030  @VIC_VEC_ADDR 
    STR R1, [R0]        @Store any value to acknowledge the interruption
    B irq_done          @Finish IRQ

