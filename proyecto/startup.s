/* startup.s */

.section .text
.syntax unified
.code 32
.globl _start

_start:
    ldr sp, =_stack_top        @ Inicializa el puntero de pila
    ldr r0, =vector_table
    mcr p15, 0, r0, c12, c0, 0
    bl os_main                 @ Llamada a main del os
    b .                        @ Loop here instead of hang label
    b hang                     @ Bucle infinito

hang:
    b hang

.globl PUT32
PUT32:
    str r1, [r0]               @ Escribe el valor de r1 en la dirección apuntada por r0
    bx lr

.globl GET32
GET32:
    ldr r0, [r0]               @ Lee desde la dirección apuntada por r0 y guarda en r0
    bx lr

.globl enable_irq
enable_irq:
    mrs r0, cpsr
    bic r0, r0, #0x80   @ Clear I-bit
    msr cpsr_c, r0  
    bx lr

.align 5
vector_table:
    b _start
    b .
    b .
    b .
    b .
    b .
    b irq_handler
    b .

irq_handler:
    push {r0-r12, lr}
    mov r0, sp
    bl timer_irq_handler
    mov sp, r0
    pop {r0-r12, lr}
    subs pc, lr, #4

.globl start_first_process
start_first_process:
    bl select_next_process      @ r0 = 0, obtiene el primer stack pointer
    mov sp, r0                  @ Cambia el stack pointer
    pop {r0-r12, lr}            @ Restaura el contexto (debe estar preparado por inicialize_stack)
    bx lr                       @ Salta a la función del proceso

.section .bss
.align 4
_stack_bottom:
    .skip 0x2000               @ Reserva 8KB para la pila
_stack_top:
