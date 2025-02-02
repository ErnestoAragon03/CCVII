.section .text
.global _start

// Dirección base de la tabla de vectores
.global vector_table

vector_table:
    b reset           // Dirección de reset
    b undef_instr     // Dirección de excepción por instrucción indefinida
    b swi_handler     // Dirección de excepción SWI (Software Interrupt)
    b prefetch_abort  // Dirección de excepción de aborto de búsqueda
    b data_abort      // Dirección de excepción de aborto de datos
    b reserved        // Dirección para uso reservado
    b irq_handler     // Dirección de la rutina de interrupción (IRQ)
    b fiq_handler     // Dirección de la rutina de interrupción rápida (FIQ)

// Reset
reset:
    ldr sp, =0x00020000   // Configurar el puntero de pila (en RAM)
    bl main               // Llamar a la función main
loop:
    b loop                // Bucle infinito para evitar salida

// Manejador de interrupciones IRQ
irq_handler:
    push {r0-r12, lr}     // Salvar los registros que pueden ser usados en la ISR

    // Llamar a la función de manejo de interrupción de timer en os.c
    bl irq_handler_func

    pop {r0-r12, pc}      // Restaurar los registros y volver a la ejecución normal

// Función para manejar la interrupción en os.c
irq_handler_func:
    // Aquí puedes agregar código para manejar la interrupción de timer
    // Por ejemplo, verificar la fuente de la interrupción en el VIC, limpiar el flag, etc.
    
    // Asegúrate de limpiar la interrupción en el VIC (por ejemplo, VIC_SOFTINTCLEAR en os.c)
    // Para este caso, llamas a `timer_clear_interrupt()` de os.c para limpiar la interrupción del timer.

    bx lr   // Regresar al punto de interrupción

// Otros manejadores de excepciones
undef_instr:
    b undef_instr

swi_handler:
    b swi_handler

prefetch_abort:
    b prefetch_abort

data_abort:
    b data_abort

reserved:
    b reserved

fiq_handler:
    b fiq_handler
