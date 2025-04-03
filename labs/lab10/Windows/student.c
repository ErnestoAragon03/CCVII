#include <stdio.h>
#include <stdlib.h>
#include "student.h"
#include <time.h>

// Función que simula la acción de "cruce"
DWORD WINAPI cruzar(void* arg) {
    Estudiante* estudiante = (Estudiante*)arg;

    // Generar un tiempo aleatorio entre 0 y 5 segundos
    int tiempo_espera = rand() % 6;
    printf("Estudiante %d esperando %d segundos antes de cruzar.\n", estudiante->id, tiempo_espera);
    sleep(tiempo_espera);

    // Acción de cruce
    printf("Estudiante %d está cruzando.\n", estudiante->id);

    return NULL;
}

// Función para crear un estudiante (thread)
Estudiante* crear_estudiante(int id) {
    Estudiante* estudiante = (Estudiante*)malloc(sizeof(Estudiante));
    if (estudiante == NULL) {
        perror("Error al asignar memoria para el estudiante");
        exit(EXIT_FAILURE);
    }

    estudiante->id = id;

    // Crear el hilo asociado al estudiante
    if (pthread_create(&estudiante->thread, NULL, cruzar, estudiante) != 0) {
        perror("Error al crear el hilo del estudiante");
        free(estudiante);
        exit(EXIT_FAILURE);
    }

    return estudiante;
}

// Función para destruir un estudiante (esperar a que termine su hilo)
void destruir_estudiante(Estudiante* estudiante) {
    if (estudiante == NULL) return;

    // Esperar a que el hilo termine
    pthread_join(estudiante->thread, NULL);

    // Liberar la memoria asignada
    free(estudiante);
}