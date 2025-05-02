.section .text
.global _start
.global main

_start:
    ldr sp, =_stack_top       // Usar stack definido por el linker
    bl main                   // Llamar a la función main
halt:
    b halt                    // Loop infinito
