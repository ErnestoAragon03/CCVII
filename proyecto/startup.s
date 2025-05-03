.section .text
.syntax unified
.code 32

.global _start

_start:
    ldr sp, =_stack_top        @ Inicializa la pila

    bl os_main                 @ Inicializa el sistema (UART, Timer, etc.)
    bl app_main                @ Ejecuta la aplicación principal

    b hang

hang:
    b hang

/******************************
 * Funciones auxiliares HW
 ******************************/
.global PUT32
PUT32:
    str r1, [r0]
    bx lr

.global GET32
GET32:
    ldr r0, [r0]
    bx lr

/******************************
 * Vectores de excepción
 ******************************/
.section .vectors, "a"
.global _vectors

_vectors:
    LDR PC, =_start           @ Reset
    LDR PC, =undef_handler    @ Undefined Instruction
    LDR PC, =swi_handler      @ Software Interrupt
    LDR PC, =prefetch_abort   @ Prefetch Abort
    LDR PC, =data_abort       @ Data Abort
    .word 0                   @ Reserved
    LDR PC, =irq_handler      @ IRQ
    LDR PC, =fiq_handler      @ FIQ

/******************************
 * Handlers de excepción
 ******************************/
.section .text

.extern isr_timer2           @ Permitimos llamar a la función en C

irq_handler:
    bl isr_timer2            @ Llama al manejador de IRQ del Timer2
    subs pc, lr, #4          @ Vuelve de la interrupción (modo ARM)

undef_handler:    b undef_handler
swi_handler:      b swi_handler
prefetch_abort:   b prefetch_abort
data_abort:       b data_abort
fiq_handler:      b fiq_handler

/******************************
 * Reserva para la pila
 ******************************/
.section .bss
.align 4

_stack_bottom:
    .skip 0x1000             @ 4KB de pila

_stack_top:
