#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include "bridge.h"
#include "student.h"

#define MAX_ESTUDIANTES 100

// Función principal
int main() {
    int num_estudiantes;
    printf("Type the number of students: ");
    scanf("%d", &num_estudiantes);
    if(num_estudiantes <= 0) {
        printf("The max value of students is %d.\n", MAX_ESTUDIANTES);
        return 1;
    }

    //Inicializar el puente
    Bridge bridge;
    bridge.crossingCount = 0;
    InitializeCriticalSection(&bridge.lock);
    InitializeConditionVariable(&bridge.canCross);

    //Crear los estudiantes
    HANDLE threads[MAX_ESTUDIANTES];
    for (int i = 0; i < num_estudiantes; i++) {
        Estudiante* estudiante = (Estudiante*)malloc(sizeof(Estudiante));
        if (estudiante == NULL) {
            perror("Error al asignar memoria para el estudiante");
            exit(EXIT_FAILURE);
        }
        estudiante->id = i + 1;
        estudiante->bridge = &bridge;
        estudiante->direction = rand() % 2 == 0 ? LEFT : RIGHT; // Dirección aleatoria // Asignar dirección aleatoria

        // Crear el hilo asociado al estudiante
        threads[i] = CreateThread(NULL, 0, cruzar, estudiante, 0, NULL);
        if (threads[i] == NULL) {
            perror("Error al crear el hilo del estudiante");
            free(estudiante);
            exit(EXIT_FAILURE);
        }

        //Simular tiempos diferentes para cada estudiante
        Sleep((rand() % 2 + 1) * 1000); // Esperar un tiempo aleatorio entre 1 y 2 segundos
    }

    // Esperar a que todos los estudiantes terminen
    WaitForMultipleObjects(num_estudiantes, threads, TRUE, INFINITE);

    // Cerrar los hilos
    for (int i = 0; i < num_estudiantes; i++) {
        CloseHandle(threads[i]);    // Cerrar el handle del hilo
    }
    DeleteCriticalSection(&bridge.lock); // Liberar la sección crítica
    
    printf("All students have crossed.\n");
    printf("Average waiting time: %.2f seconds\n", bridge.averageWaitingTime / num_estudiantes); // Imprimir el tiempo promedio de espera
    return 0;
}