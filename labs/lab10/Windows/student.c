#include <stdio.h>
#include <stdlib.h>
#include "student.h"
#include "bridge.h"
#include <time.h>

// Función que simula la acción de "cruce"
DWORD WINAPI cruzar(void* arg) {
    Estudiante* estudiante = (Estudiante*)arg;

    //Intentar cruzar el puente
    printf("Inge %d arrives wanting to go %s\n", estudiante->id, estudiante->direction == RIGHT ? "Right" : "Left");
    accessBridge(estudiante->bridge, estudiante->direction);

    //Simular el tiempo de cruce
    printf("Inge %d crosses to the %s (on bridge: %d).\n", estudiante->id, estudiante->direction == RIGHT ? "Right" : "Left", estudiante->bridge->crossingCount);   
    // Generar un tiempo aleatorio entre 0 y 3 segundos
    Sleep((rand() % 3 + 1) * 1000);

    // Salir del puente
    printf("Inge %d exits bridge. (on bridge: %d)\n", estudiante->id, estudiante->bridge->crossingCount);
    exitBridge(estudiante->bridge);

    free(estudiante); // Liberar la memoria del estudiante después de cruzar
    return 0;
}

/*
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
*/