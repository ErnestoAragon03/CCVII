# Lab006 - Hilos en Linux

## Video de YouTube
[Ver video](||)
---

---
## **Colaboradores**
- [Poncho Ajmv](https://github.com/poncho-ajmv)
---

## Resumen de la Implementación
Este laboratorio se enfocó en la introducción a los hilos en entornos Linux utilizando **POSIX threads (pthreads)**. A través del desarrollo de una aplicación multi-hilo, se demostró cómo crear y gestionar hilos para mejorar el rendimiento de una aplicación que procesa archivos de texto en paralelo.

---

## Desarrollo del Procesador de Archivos Multi-Hilo
Se implementó un programa que procesa varios archivos de texto simultáneamente, aprovechando la concurrencia de los hilos. El programa recibe como entrada una lista de archivos y crea un hilo independiente para cada uno de ellos. Cada hilo se encarga de:

1. Abrir su respectivo archivo en modo lectura.
2. Contar el número de líneas del archivo.
3. Imprimir el nombre del archivo junto con su conteo de líneas, incluyendo el identificador del hilo.
4. Finalizar su ejecución.

Para garantizar que todos los hilos finalicen antes de que el programa principal termine, se utilizó `pthread_join()`, asegurando una sincronización adecuada entre los hilos.

---

## Estructura del Proyecto
# Estructura del Proyecto

Lab_Threads/
│
├─ Linux/
│   ├─ main_linux.c        # Procesador de archivos multi-hilo con pthreads
│   ├─ Makefile            # Script de compilación
│   └─ run.sh              # Script para compilar y ejecutar el programa
│
└─ Instructions/
    └─ Lab_Instructions.pdf # Documento con instrucciones del laboratorio


---

## Implementación de la Concurrencia y Sincronización
Para cada archivo proporcionado, se creó un hilo utilizando `pthread_create()`. La estructura básica del programa incluyó:

- La inicialización de un arreglo de identificadores de hilos.
- La creación de hilos, asignando a cada uno la tarea de procesar un archivo.
- La utilización de `pthread_join()` para esperar la finalización de todos los hilos antes de calcular y mostrar el conteo total de líneas procesadas.

### Ejemplo de Salida:
[Thread ID: 1234] archivo1.txt tiene 100 líneas. [Thread ID: 5678] archivo2.txt tiene 200 líneas. 
[Thread ID: 9101] archivo3.txt tiene 50 líneas. Total de líneas contadas: 350


---

## Experimentos con Concurrencia
Para demostrar la naturaleza concurrente de los hilos, se realizaron varias pruebas:

- Introducir retardos artificiales con `sleep()` para simular tiempos de procesamiento variables.
- Cambiar el orden de los archivos proporcionados para analizar el impacto en la ejecución.
- Observar que el orden de creación de los hilos no necesariamente coincide con el orden en que terminan su ejecución.

---

## Manejo de Casos Especiales
Durante la ejecución del programa, se verificó su comportamiento ante diversas condiciones:

- **Archivos vacíos:** Se mostró un conteo de cero líneas.
- **Archivos con solo espacios en blanco:** Se manejaron de manera similar a los vacíos.
- **Archivos inexistentes:** Se mostró un mensaje de error sin afectar la ejecución de los otros hilos.

---

## Conclusiones
A través de este laboratorio se logró:
- Comprender cómo funcionan los hilos y en qué se diferencian de los procesos.
- Implementar una solución multi-hilo para mejorar el rendimiento de una aplicación.
- Aplicar `pthread_join()` para garantizar la sincronización entre el hilo principal y los hilos secundarios.
- Experimentar con concurrencia y observar cómo los hilos ejecutan tareas de manera paralela.

Este ejercicio brindó una base sólida para trabajar con hilos en entornos Linux y optimizar programas que requieren procesamiento paralelo.
