#ifndef student_h
#define student_h

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h> // Para Windows, si se usa Linux, cambiar a <unistd.h> y eliminar <windows.h>
#include "bridge.h" // Incluir el archivo de cabecera del puente

// Estructura para representar un estudiante
typedef struct {
    int id;                 // Identificador único del estudiante
    Direction direction; // Dirección en la que el estudiante va a cruzar
    Bridge* bridge; // Puntero al puente
} Estudiante;

DWORD WINAPI cruzar(LPVOID arg); // Función que simula la acción de "cruce"


#endif