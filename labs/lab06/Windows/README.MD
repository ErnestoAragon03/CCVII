# Lab06 — Threads (Windows)

## Objetivo
Crear un programa capaz de recibir como argumentos en la consola de comandos los PATHS a archivos de texto (.txt), abrirlos y contar la cantidad de líneas que cada uno posee, todo esto haciendo uso de Threads.  Un thread por cada archivo pasado.

# Tareas 
## Tarea 1
- Crear un programa que tome una lista de paths como argumentos de comando de línea
- Crear un Thread para cada archivo de input
- Cada Threa debe:
    1. Abrir el archivo en modo lectura
    2. Contar el número de líneas del archivo
    3. Imprimi el nombre del archivo y el recuento de líneas en el formato: [Thread ID: 1234] file1.txt has 150 lines.
- El Thread main deberá
    1. Esperar a que todos los threads terminen usando WaitForMultipleObjects() (para windows)
    2. Resumir los resultados, desplegando el conteo total de líneas de todos los archivos.

## Tarea 2 
- Explicar La importancia del Thread joining
El Thread joining porque previene que el Thread principal (el proceso padre) termine antes de tiempo.  Además de que ayuda con la sincronización, asegurando que todos los Threads hijos hayan terminado y su resultado esté listo para usarse en el resumen realizado por el Thread main.
- Para Windows, usar WaitForSingleObject(thread_name, INFINITE);
- Probar la aplicación con diferente número de archivos y observar el comportamiento

## Tarea 3
- Introducir sleep delays (Sleep() en windows) para simular tiempo de procesamiento en el archivo.
- Mezclar el orden de los archivos para ver como los threads manejan procesamiento concurrente.
- Observar como el orden de la creación del Thread puede no coincidir con el orden de ejecución, esto demuestra verdadera concurrencia.

### Link al video del lab:
[Ver video](https://youtu.be/wI6ZCWsVZU4)