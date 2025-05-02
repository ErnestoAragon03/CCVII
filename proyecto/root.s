    .global _start
    .text

_start:
    LDR sp, =_stack_top       // Configurar pila
    BL main                   // Llamar a la función principal en C

    B .                       // Bucle infinito (por si main retorna)
