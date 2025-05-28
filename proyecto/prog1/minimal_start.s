.section .text
.global _start

_start:
    bl main     @ Llama a main del proceso (debe estar en p1.c o p2.c)
    b .         @ Loop infinito

@ Stub para PUT32 y GET32 usados por uart.c
.global PUT32
PUT32:
    str r1, [r0]
    bx lr

.global GET32
GET32:
    ldr r0, [r0]
    bx lr
