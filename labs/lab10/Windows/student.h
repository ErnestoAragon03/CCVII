#ifndef student_h
#define student_h

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h> // Para Windows, si se usa Linux, cambiar a <unistd.h> y eliminar <windows.h>

// Estructura para representar un estudiante
typedef struct {
    int id;                 // Identificador único del estudiante
    HANDLE thread;       // Hilo asociado al estudiante
} Estudiante;

DWORD WINAPI cruzar(LPVOID arg); // Función que simula la acción de "cruce"
Estudiante* crear_estudiante(int id); // Función para crear un estudiante (thread)
void destruir_estudiante(Estudiante* estudiante); // Función para destruir un estudiante (esperar a que termine su hilo)


#endif