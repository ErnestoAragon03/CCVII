/* startup.s */

.section .text
.syntax unified
.code 32
.globl _start
.equ OFFSET_SP, 12
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
    mrs r0, cpsr
    push {r0}

    ldr r1, =current_process   @ Guardar SP actual en PCB actual_process->sp
    ldr r1, [r1]
    cmp r1, #0
    beq skip_save_sp
    str sp, [r1, #OFFSET_SP]

skip_save_sp:
    bl timer_irq_handler         @ Solo limpia TISR y notifica INTC

    bl select_next_process     @ Llamar al scheduler
    
   
    ldr r1, =current_process    @ Cargar SP del nuevo proceso actual
    ldr r1, [r1]
    cmp r1, #0
    beq skip_load_sp
    ldr sp, [r1, #OFFSET_SP]

skip_load_sp:
    pop {r0}
    msr cpsr_fsxc, r0
    pop {r0-r12, lr}
    subs pc, lr, #4

.section .bss
.align 4
_stack_bottom:
    .skip 0x2000               @ Reserva 8KB para la pila
_stack_top:
