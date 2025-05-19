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
    sub sp, sp, #68              @ Espacio para contexto (17 * 4 bytes)
    stmia sp, {r0-r12, lr}       @ Guarda r0-r12 y lr (13 + 1 = 14 registros)
    mrs r0, spsr                 @ Obtiene el CPSR del modo usuario
    str r0, [sp, #64]            @ Guarda CPSR en el último espacio

    mov r0, sp                   @ Pasar dirección del stack actual
    bl save_context              @ Guardar contexto actual

    bl select_next_process       @ Cambiar current_pid
    bl restore_context           @ Devuelve nuevo stack pointer
    mov sp, r0                   @ Restaurar SP

    ldmia sp, {r0-r12, lr}       @ Cargar registros
    ldr r0, [sp, #64]            @ Cargar CPSR simulado
    msr spsr_cxsf, r0            @ Restaurar SPSR
    add sp, sp, #68              @ Ajustar stack

    ldr r0, =0x48040028          @ TISR: limpiar flag de interrupción
    mov r1, #0x2
    str r1, [r0]

    ldr r0, =0x48200048          @ INTC_CONTROL: señal fin de IRQ
    mov r1, #0x1
    str r1, [r0]

    subs pc, lr, #4              @ Regreso a modo usuario




.section .bss
.align 4
_stack_bottom:
    .skip 0x2000               @ Reserva 8KB para la pila
_stack_top:
