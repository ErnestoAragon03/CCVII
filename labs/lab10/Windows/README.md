# Lab10 — Condition Variables (Windows)

## Objetivo
Crear un programa que simule un grupo de estudiantes qeu deben de cruzar un puente de manera ordenada y con un límite de usuarios en el puente al mismo tiempo.  Respetando la dirección a la que se dirige el grupo (no pueden ir dos estudiantes a la vez en direcciones contrarias).

# Tareas 
## Tarea 1 — Simulate Student Threads: 
Para completar esta tarea se implemento una struct de c llamada Estudiantes, la cual está definida en el archivo student.h.  Los elementos de la estructura son los siguientes:
- int idd: Es el ID del estudiante
- Direction direction: Básicamente es un tipo de dato que definí para evitar tener que estar usando 0 y 1 y simplemente usar RIGHT y LEFT, mucho más sencillo.  Indica la dirección en la que se quiere dirigir el estudiante
- Bridge* bridge: Puntero a la estructura que definí al bridge.  Básicamente sirve para saber a donde hacer referencia para los métodos y algunas variables

Además también se creó un metodo llamado "cruzar" para indicarle a un estudiante que cruce el puente (sin importar si puede o no aún, de eso se encarga el Bridge con su variable de condición).

## Tarea 2 — Implement a thread-safe bridge structure:
Aquí se definió una estructura en el archivo bridge.h llamada "Bridge" la cual tiene los siguientes campos:
- int crossingCount: Indica la cantidad de personas actualmente en el puente
- Direction currentDirection: Indica la dirección que tiene el puente actualmente
- CRITICAL_SECTION lock: Lock para proteger el acceso a la estructura
- CONDITION_VARIABLE canCross: El corazon del sistema de cruce
- double averageWaitingTime: Forma sencilla para recopilar el total de waitingTime de todos los estudiantes para luego sacar el promedio.

## Tarea 3 — Enforce bridge constraints:
Se asegurarón las reglas definidas en el bridge, haciendo uso de la ConditionVariable, se revisa que no se rompa ninguna de las reglas, que no hayan más de 4 estudiantes cruzando ya y que el estudiante tenga la misma dirección que el grupo que actualmente esta cruzando.  En caso de no cumplirse alguna de las dos entonces mandamos a dormir al Thread con SleepConditionVariableCS.

## Tarea 4 — Log activity
Simplemente mostramos los 3 estados en los que un estudiante se puede encontrar (llegando, cruzando o saliendo) en los momentos correspondientes, ya sea en el archivo student.c o en el bridge.c.

## Tareas Extra
Se cumplieron las tareas extras haciendo uso de una condicional cada que un estudiante sale del puente, la cual indica que en caso de que ya no hayan más estudiantes cruzando el puente entonces, independiente de la cantidad de estudiantes que esten en cada lado del puente, se fuerza un cambio de dirección y allí es cuando se despierta a todos los threads que estaban durmiendo, esto con la función WakeConditionVariable.  De esta forma evitamos el Starving en los procesos que esten esperando de un lado del puente mientras que al otro se le da todo el tiempo del proceso.

### Link al video del lab:
[Ver video](https://youtu.be/Y0RPa8FmjC8)