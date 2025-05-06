/* startup.s */

.section .text
.syntax unified
.code 32
.globl _start

_start:
    ldr sp, =_stack_top        @ Inicializa el puntero de pila
    bl main                    @ Llama a main
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

.section .bss
.align 4

_stack_bottom:
    .skip 0x1000               @ Reserva 4KB para la pila

_stack_top:
