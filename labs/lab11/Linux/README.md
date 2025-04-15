# Lab011 - Simulación de Algoritmos de Planificación de CPU

## Video de YouTube
[Ver video](||)
---

---
## **Colaboradores**
- [Poncho Ajmv](https://github.com/poncho-ajmv)
---

## Descripción

Este proyecto implementa y compara cuatro algoritmos de planificación de CPU en lenguaje C bajo Linux. Los algoritmos incluidos son:

- **FIFO (First-In, First-Out / FCFS)**  
- **Round Robin (RR)** con quantum fijo de 2 unidades de tiempo  
- **SJF (Shortest Job First)** – No expropiativo  
- **SRTF (Shortest Remaining Time First)** – Expropiativo  

El objetivo es evaluar el comportamiento de cada uno sobre un mismo conjunto de datos aleatorios, incluyendo tiempos de espera, tiempos de retorno y representación visual.

---

## Escenario

Se simula un sistema con un solo núcleo de CPU que debe gestionar múltiples procesos. Cada proceso tiene:

- Un tiempo de llegada aleatorio
- Una ráfaga de CPU aleatoria (tiempo necesario de ejecución)
- Un identificador único (PID)

Los procesos se planifican y ejecutan según las políticas definidas por los algoritmos anteriores, tal como se haría en un sistema operativo real.

---

## Estructura del Código

- Se define una estructura `Process` que almacena toda la información necesaria para cada hilo de ejecución (PID, llegada, ráfaga, tiempo restante, comienzo, finalización, tiempos de espera y retorno).
- Se simulan eventos del sistema y se registran en consola en el formato `[t=tiempo] evento`.
- Cada algoritmo opera sobre una copia de los procesos originales, reiniciando sus campos antes de la ejecución.
- Se calcula y muestra:
  - Tiempo de espera (start - arrival)
  - Tiempo de retorno (completion - arrival)
  - Promedios de cada uno

---

## Algoritmos Implementados

### FIFO (First-In-First-Out)

- Los procesos se ordenan según su tiempo de llegada.
- Se ejecutan en orden, sin interrupciones.
- Si el proceso llega más tarde que el tiempo actual, la CPU permanece inactiva hasta que llegue.

### Round Robin (Quantum = 2)

- Se utiliza una cola para administrar procesos listos.
- Cada proceso se ejecuta por un máximo de 2 unidades de tiempo.
- Si no termina, se reencola con el tiempo restante.
- Se registran los eventos de inicio, pausa (preempaquetado) y finalización.

### SJF (Shortest Job First - No expropiativo)

- Se selecciona el proceso con la ráfaga más corta entre los que han llegado.
- Se ejecuta completamente antes de pasar al siguiente.
- No hay cambio de proceso una vez que comienza la ejecución.

### SRTF (Shortest Remaining Time First - Expropiativo)

- Se ejecuta el proceso con menor ráfaga restante entre los disponibles.
- Si llega uno más corto, el actual se preemptea.
- El tiempo avanza unidad por unidad para detectar los cambios.

---

## Extras Implementados 

- Gantt Chart en ASCI se genera un diagrama de ejecución para cada algoritmo 
- En el video se adjunta los extas del lab 01 y 05


---

## Compilación y Ejecución

Debemos de darle permiso con chmod+x run.sh 
ejecutar ./run.sh


