#ifndef bridge_h
#define bridge_h

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

typedef enum{
    RIGHT = 0,
    LEFT = 1,
} Direction;

typedef struct {
    int crossingCount;      // Contador para número de estudiantes que cruzaron
    Direction currentDirection; // Dirección actual del cruce
    CRITICAL_SECTION lock; // Lock para proteger el acceso a la estructura
    CONDITION_VARIABLE canCross; // Variable de condición para sincronización
    double averageWaitingTime; // Tiempo promedio de espera
} Bridge;

// Función para que un estudiante intente acceder al puente
void accessBridge(Bridge* bridge, Direction direction);

// Función para que un estudiante salga del puente
void exitBridge(Bridge* bridge);

#endif