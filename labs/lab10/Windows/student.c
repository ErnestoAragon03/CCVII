#include <stdio.h>
#include <stdlib.h>
#include "student.h"
#include "bridge.h"
#include <time.h>

// Función que simula la acción de "cruce"
DWORD WINAPI cruzar(void* arg) {
    Estudiante* estudiante = (Estudiante*)arg;

    // Generar un tiempo aleatorio entre 0 y 5 segundos
    Sleep((rand() % 5 + 1) * 1000);

    //Intentar cruzar el puente
    printf("Inge %d arrives wanting to go %s\n", estudiante->id, estudiante->direction == RIGHT ? "Right" : "Left");
    accessBridge(estudiante->bridge, estudiante->direction);

    //Simular el tiempo de cruce
    printf("Inge %d crosses to the %s (on bridge: %d).\n", estudiante->id, estudiante->direction == RIGHT ? "Right" : "Left", estudiante->bridge->crossingCount);   
    // Generar un tiempo aleatorio entre 1 y 3 segundos
    Sleep((rand() % 3 + 1) * 1000);

    // Salir del puente
    printf("Inge %d exits bridge. (on bridge: %d)\n", estudiante->id, estudiante->bridge->crossingCount);
    exitBridge(estudiante->bridge);

    free(estudiante); // Liberar la memoria del estudiante después de cruzar
    return 0;
}