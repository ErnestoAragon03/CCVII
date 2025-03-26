#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_PARKING_SPOTS 3
#define NUM_CARS 10

HANDLE parkingSpotsSemaphore;
CRITICAL_SECTION logCriticalSection;

void logEvent(const char* event, int carId, float waiting_time) {
    EnterCriticalSection(&logCriticalSection);
    SYSTEMTIME time;
    GetLocalTime(&time);
    const char* daysOfWeek[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
    if(waiting_time >= 0.0) {
        printf("[%s %02d %02d %02d:%02d:%02d %04d] Car %d: %s (waited %.2f seconds)\n", 
            daysOfWeek[time.wDayOfWeek], time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond, time.wYear, carId, event, waiting_time);
    } else {
        printf("[%s %02d %02d %02d:%02d:%02d %04d] Car %d: %s\n", 
            daysOfWeek[time.wDayOfWeek], time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond, time.wYear, carId, event);
    }
    LeaveCriticalSection(&logCriticalSection);
}

DWORD WINAPI carThread(LPVOID param) {
    srand(GetCurrentThreadId());
    int carId = *(int*)param;

    // Arrive
    logEvent("Arrived at parking lot", carId, -1);

    //Start counting time for parking
    clock_t start = clock();

    // Wait for a parking spot
    WaitForSingleObject(parkingSpotsSemaphore, INFINITE);

    //End counting time for parking
    clock_t end = clock();
    float waiting_time = (float)(end - start) / CLOCKS_PER_SEC; // Convert to seconds
    
    // Park
    int parkingDuration = (rand() % 5) + 1;
    logEvent("Parked succesfully", carId, waiting_time);
    Sleep(parkingDuration * 1000);

    // Leave
    logEvent("Leaving", carId, -1);
    ReleaseSemaphore(parkingSpotsSemaphore, 1, NULL);

    return 0;
}

int main() {
    // Initialize semaphore and critical section
    parkingSpotsSemaphore = CreateSemaphore(NULL,   //Atributos de seguridad por defecto
        MAX_PARKING_SPOTS,                          //Valor inicial del semáforo
        MAX_PARKING_SPOTS,                          //Valor máximo del semáforo       
        NULL                                        //Nombre del semáforo (NULL = sin nombre)
    );
    InitializeCriticalSection(&logCriticalSection);

    HANDLE carThreads[NUM_CARS];

    // Create car threads
    for (int i = 0; i < NUM_CARS; i++) {
        int* carId = (int*)malloc(sizeof(int));
        *carId = i + 1;
        carThreads[i] = CreateThread(NULL, 0, carThread, carId, 0, NULL);
    }

    // Wait for all car threads to finish
    WaitForMultipleObjects(NUM_CARS, carThreads, TRUE, INFINITE);

    // Clean up
    for (int i = 0; i < NUM_CARS; i++) {
        CloseHandle(carThreads[i]);
    }
    CloseHandle(parkingSpotsSemaphore);
    DeleteCriticalSection(&logCriticalSection);

    return 0;
}