# Proyecto de Multiprogramación en BeagleBone Black

## 🧑‍💻 Colaboradores
- [Ernesto Aragon](https://github.com/ErnestoAragon03)
- [Christian Sosa](https://github.com/ChristianSosa22)
- [Poncho Ajmv](https://github.com/poncho-ajmv)

## 📝 Descripción General

Este proyecto implementa un sistema operativo bare-metal con multiprogramación para la plataforma BeagleBone Black. El sistema carga tres bloques de código en memoria: un sistema operativo básico, y dos procesos de usuario (P1 y P2). El OS gestiona los procesos a través de interrupciones periódicas generadas por el temporizador DMTimer2 del AM335x, aplicando un planificador Round-Robin.

### 🧠 Funcionalidad
- Proceso P1 imprime en bucle letras minúsculas de la 'a' a la 'z'.
- Proceso P2 imprime en bucle los números del 0 al 9.
- El sistema operativo realiza cambios de contexto cada 1 segundo mediante interrupciones, alternando la ejecución entre ambos procesos.

## ⚙️ Componentes del Proyecto

- `root.s` y `os.c`: Inicialización de hardware (UART, Timer, INTC) y manejo de interrupciones.
- `stdio.c` y `string.c`: Funciones auxiliares para I/O y manejo de cadenas.
- `process1.c`: Imprime letras en bucle.
- `process2.c`: Imprime números en bucle.
- `linker.ld`: Script para asignar posiciones fijas de memoria a cada módulo y pila separada.
- `build_and_run.sh`: Script para compilar y cargar el sistema en la BeagleBone Black.

## 🗂️ Objetivos del Proyecto

1. Implementar un sistema multiprogramado sin MMU con 3 bloques en memoria.
2. Configurar UART0, DMTimer2 e INTC para manejo de interrupciones.
3. Desarrollar un planificador Round-Robin con cambios de contexto cada 1 segundo.
4. Mantener separación de pilas y preservar el estado de los procesos entre cambios.
5. Generar una salida intercalada en consola (e.g., `0 1 2 3 4 a b c d e ...`).

## 📆 Cronograma de Desarrollo

| Fecha   | Tarea                                                                 |
|---------|-----------------------------------------------------------------------|
| 25/04   | Implementar procesos P1 y P2 de forma autónoma (portar Lab 2).        |
| 02/05   | Integrar Lab 3: manejar interrupciones y conmutar entre procesos.     |
| 09/05   | Tener al menos 2/3 del proyecto implementado y funcionando.           |
| 16/05   | Finalizar la implementación y realizar ajustes finales.               |
| 28/05   | Entrega presencial final del proyecto.                                |

## 📌 Notas Técnicas

- Las direcciones de memoria para cada módulo están definidas en el linker script.
- Cada proceso tiene su pila dedicada para evitar solapamientos.
- El sistema no utiliza sistema operativo de base: corre completamente en bare-metal.
- Se utiliza `PUT32` y `GET32` para acceso directo a registros de hardware.

## 🧪 Pruebas

- Verificación del cambio de contexto observando la salida intercalada.
- Pruebas unitarias para UART y Timer antes de integrar el planificador.
- Pruebas de estabilidad en ejecución continua de los procesos.


## Implementación

### timer.c
Este archivo está destinado a las funciones relacionadas con el timer del BeagleBoard.  Contiene las siguientes funciones:

- void timer_init(void):
- void timer_irq_handler(void):


### uart.c
Este archivo está destinado a la comunicación UART con la BeagleBoard.  Tiene las siguientes funciones:

- void uart_send(unsigned char): Función encargada de
- void uart_hex(unsigned int):
- void PRINT(const char*):

### scheduler.c
Este archivo se encarga de todas las funciones necesarias para el scheduler de procesos del os.  Cuenta con las siguientes funciones:

- 

### pcb.c 
Archivo encargado de manejar los Process Content Blocks (PCBs).  El PCB es una estructura conformada de la siguiente forma:
- unsigned int pid:  Process ID, identificador del proceso
- unsigned int priority: Prioridad del proceso, en caso se quiera crear un scheduler con prioridades (escalabilidad a futuro)
- unsigned int stack_pointer: Puntero al stack del proceso por si es necesario para un context switching o alguna otra aplicación (escalabilidad a futuro)
- state_process state:  Define el estado en el que se encuentra el proceso, state_process es una estructura propia de mi os que puede tener uno de los siguientes valores:
- - READY
- - RUNNING
- - BLOCKED
- - TERMINATED

En el archivo de pcb.c además se definen funciones para interactuar con los PCBs, estas son:
- void create_table(void): Crea una nueva tabla de PCBs, aquí es donde se almacenan todos los PCBs de los procesos actuales.
- int create_process(void (*function)(void)): Crea un PCB nuevo para un proceso, el único argumento necesario (por el momento) es la función o handler del proceso.  El resto se define de manera automática.  Retorna el PID del PCB recién creado
- void start_process(unsigned int pid):  Inicia la ejecución de proceso.  Recibe como argumento el PID del proceso que se quiere ejecutar.  Mientras se ejecuta el estado (state_process) del PCB se cambia a RUNNING y una vez se concluye la ejecución del proceso se cambia a TERMINATED.  No tiene ningún retorno.