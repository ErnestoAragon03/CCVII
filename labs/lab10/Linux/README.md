# Simulación del Puente Zombie 🧟‍♂️🌉

## Video de YouTube  
[Ver video](|https://youtu.be/a3HW0pPwI2A|)

---

## **Colaboradores**  
- [Poncho Ajmv](https://github.com/poncho-ajmv)

---

## Resumen  
Este proyecto simula un escenario de concurrencia en el contexto de un apocalipsis zombie, donde estudiantes universitarios deben cruzar un puente estrecho para llegar a una zona segura. Utiliza **hilos (threads)**, **variables de condición**, y **bloqueos mutuos (mutexes)** para garantizar que el cruce sea seguro, justo, y libre de inanición.

---

## Descripción  
Durante un brote zombie en la universidad, los estudiantes deben atravesar un **puente unidireccional** con una capacidad máxima de 4 personas. El puente solo puede ser utilizado en una dirección a la vez. Los estudiantes (hilos) intentan cruzar en direcciones aleatorias, y el sistema debe garantizar:

- **Seguridad:** Nunca más de 4 personas en el puente.  
- **Dirección:** Solo estudiantes en una misma dirección a la vez.  
- **Equidad:** Prevención de inanición para estudiantes esperando en la dirección opuesta.

Se utiliza una **interfaz visual en consola** para representar gráficamente el escenario, mostrando universidad, puente, zona segura y una horda de zombies debajo.

---

## Objetivos  

- Implementar sincronización de hilos usando `pthread_mutex_t` y `pthread_cond_t`.  
- Simular acceso controlado a una sección crítica (el puente).  
- Implementar prevención de inanición.  
- Visualizar el comportamiento concurrente usando gráficos en terminal.  
- Mostrar estadísticas finales del proceso.

---

## Estructura del Proyecto  

```plaintext
Lab_10
│
├─ LabBase/
│   ├─ main               → Ejecutable base  
│   ├─ main.c             → Lógica principal del puente  
│   ├─ makefile           → Script de compilación  
│   └─ run.sh             → Script para compilar y ejecutar  
│
├─ LabVisualTerminal/
│   ├─ main.c             → Código visual con lógica de sincronización  
│   ├─ main.h             → Header con constantes y prototipos  
│   ├─ main.o             → Objeto compilado  
│   ├─ makefile           → Compilador del entorno visual  
│   ├─ run.sh             → Ejecuta la simulación visual  
│   └─ zombie_bridge      → Ejecutable de simulación visual  
│
└─ 010ConditionVariables.pdf → Documento de instrucciones del laboratorio

##Ejecutar 
Solo se debe de dar permiso de ejecucion en el sh
chmod +x run.sh

y despues se ejecuta con el ./run.sh

## Ejemplo de Salida
Inge 10 cruza hacia Derecha (en puente: 1)
Inge 04 llega queriendo ir a Derecha
Inge 04 cruza hacia Derecha (en puente: 2)
Inge 07 llega queriendo ir a Izquierda
Inge 08 llega queriendo ir a Izquierda
Inge 04 sale del puente (en puente: 1)
Inge 10 sale del puente (en puente: 0)
Inge 07 cruza hacia Izquierda (en puente: 1)
Inge 08 cruza hacia Izquierda (en puente: 
