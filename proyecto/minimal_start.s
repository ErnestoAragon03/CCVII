.section .text
.global _start

_start:
    bl main     @ Llama a la función main del proceso (main_p1 o main_p2)
    b .         @ Loop infinito al terminar

@ Stub para PUT32 y GET32 usados por uart.c
.global PUT32
PUT32:
    str r1, [r0]
    bx lr

.global GET32
GET32:
    ldr r0, [r0]
    bx lr
