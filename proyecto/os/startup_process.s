/* startup_process.s */

.section .text
.syntax unified
.code 32

.globl main_p1
.globl main_p2
.globl main_p3

/* Estas funciones deben estar definidas en p1.c y p2.c respectivamente */

.globl PUT32
PUT32:
    str r1, [r0]       @ Escribe el valor de r1 en la dirección apuntada por r0
    bx lr

.globl GET32
GET32:
    ldr r0, [r0]       @ Lee desde la dirección apuntada por r0 y guarda en r0
    bx lr

.section .bss
.align 4
_stack_bottom:
    .skip 0x2000       @ Reserva 8KB para la pila
_stack_top:
