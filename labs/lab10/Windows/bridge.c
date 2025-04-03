#include "bridge.h"
#include <windows.h>
#include <time.h>

void accessBridge(Bridge* bridge, Direction direction){

    EnterCriticalSection(&bridge->lock); // Entrar en la sección crítica
    clock_t start = clock(); // Iniciar el temporizador
    //Esperar hasta que el estudiante pueda cruzar (si hay más de 3 cruzando o se va en dirección opuesta)
    while (bridge->crossingCount > 3 || bridge->currentDirection != direction) {
        SleepConditionVariableCS(&bridge->canCross, &bridge->lock, 800);
    }

    clock_t end = clock(); // Finalizar el temporizador
    double waitingTime = (double)(end - start) / CLOCKS_PER_SEC; // Calcular el tiempo transcurrido
    bridge->averageWaitingTime += waitingTime; // Acumular el tiempo de espera promedio
    //Actualizar la dirección y el contador de estudiantes cruzando
    bridge->currentDirection = direction;
    bridge->crossingCount++;

    LeaveCriticalSection(&bridge->lock); // Salir de la sección crítica
}

void exitBridge(Bridge* bridge){
    EnterCriticalSection(&bridge->lock); // Entrar en la sección crítica

    // Decrementar el contador de estudiantes cruzando
    bridge->crossingCount--;

    // Si no hay más estudiantes cruzando, notificar a los demás
    if (bridge->crossingCount == 0) {
        bridge->currentDirection = (bridge->currentDirection == RIGHT) ? LEFT : RIGHT; // Cambiar la dirección
        // Despertar a los estudiantes que están esperando para cruzar
        WakeConditionVariable(&bridge->canCross);
    }

    LeaveCriticalSection(&bridge->lock); // Salir de la sección crítica
}