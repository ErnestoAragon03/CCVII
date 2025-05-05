.section .text
.syntax unified
.code 32

.global _start


_start:
    ldr sp, =_stack_top

    ldr r0, =_vectors          @ Dirección en RAM donde están los vectores
    ldr r1, =0x82000000        @ Dirección en RAM donde queremos ubicarlos (en la región válida)
    mov r2, #64                @ Tamaño de la tabla de vectores (8 entradas x 4 bytes = 32 bytes, usa 64 para margen)
copy_vectors:
    ldr r3, [r0], #4
    str r3, [r1], #4
    subs r2, r2, #4
    bne copy_vectors

    @ Desactivar vectores altos
    mrc p15, 0, r0, c1, c0, 0
    bic r0, r0, #(1 << 13)
    mcr p15, 0, r0, c1, c0, 0

    @ Establecer VBAR a la tabla de vectores copiada en RAM
    ldr r0, =0x82000000
    mcr p15, 0, r0, c12, c0, 0

    bl os_main
    bl app_main

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
    sub lr, lr, #4            @ Ajusta LR para el retorno correcto
    stmfd sp!, {r0-r12, lr}   @ Guarda todos los registros que podrían ser usados
    mrs r11, spsr             @ Guarda el SPSR (Status Register)

     @ Leer la IRQ activa
    ldr r10, =0x48200040       
    ldr r10, [r10]            @ Lee el valor de INTC_SIR_IRQ
    and r10, r10, #0x7F    @ Aplica la máscara para obtener el número de IRQ

    cmp r10, #68                @ Compara r10 (IRQ activa) con 68
    bne irq_unknown             @ Si no es 68, salta a un manejador de interrupciones desconocidas

    @ Salto a la rutina de interrupción correspondiente
    ldr pc, [pc, r10, lsl #2] @ Usa el valor de r10 como índice para la tabla
    
    nop

    @ Tabla de rutinas de interrupción
    .word irq68_handler         @ Manejo para IRQ68

irq_unknown:
    @ Manejo de interrupción desconocida (puedes hacer algo aqui, como un error o una rutina genérica)
    b irq_end

irq68_handler:
    bl isr_timer2             @ rutina de servicio y Limpia interrupción C
    b irq_end

irq_end:

    @ Permitir nuevas interrupciones: NEWIRQAGR en INTC_CONTROL
    ldr r0, =0x48200048
    mov r1, #1
    str r1, [r0]

    @ Data Synchronization Barrier (espera que todo se complete)
    mov r0, #0
    mcr p15, 0, r0, c7, c10, 4

    @ Restaurar SPSR y contexto
    msr spsr_cxsf, r11
    ldmfd sp!, {r0-r12, lr}
    subs pc, lr, #0


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
