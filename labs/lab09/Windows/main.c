#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_PARKING_SPOTS 1
#define NUM_CARS 10

// Struct to store the data of the car
typedef struct {
    int carId;
    float waiting_time;
    int priority;
} CarData;

HANDLE parkingSpotsSemaphore;
CRITICAL_SECTION logCriticalSection;

CRITICAL_SECTION priorityCriticalSection;
HANDLE priorityQueues[4][NUM_CARS];
int queueSizes[4] = {0,0,0,0};

// Counter for the number of cars that have parked
int parkedCars = 0;
// Counter for the average waiting time
float averageWaitingTime = 0.0;
// Array to store the data of the cars
CarData cars[NUM_CARS];

// Function to dequeue a thread from the priority queue
HANDLE dequeue(int priority) {
    EnterCriticalSection(&priorityCriticalSection);
    if (queueSizes[priority] == 0) {
        LeaveCriticalSection(&priorityCriticalSection);
        return NULL; // Queue is empty
    }
    HANDLE threadHandle = priorityQueues[priority][0];
    // Shift all elements to the left
    for (int i = 1; i < queueSizes[priority]; i++) {
        priorityQueues[priority][i - 1] = priorityQueues[priority][i];
    }
    queueSizes[priority]--;
    LeaveCriticalSection(&priorityCriticalSection);
    return threadHandle;
}

// Function to log an event to the console
void logEvent(const char* event, int carId, float waiting_time, int priority) {
    char* carType;
    switch(priority){
    case 0:
        carType = "Ambulance";
        break;
    case 1:
        carType = "Police";
        break;
    case 2: 
        carType= "Firefighter";
        break;
    case 3:
        carType = "Car";
        break;
    };
    EnterCriticalSection(&logCriticalSection);
    SYSTEMTIME time;
    GetLocalTime(&time);
    const char* daysOfWeek[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
    if(waiting_time >= 0.0) {
        printf("[%s %02d %02d %02d:%02d:%02d %04d] %s with ID %d: %s (waited %.2f seconds)\n", 
            daysOfWeek[time.wDayOfWeek], time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond, time.wYear, carType, carId, event, waiting_time);
    } else {
        printf("[%s %02d %02d %02d:%02d:%02d %04d] %s with ID %d: %s\n", 
            daysOfWeek[time.wDayOfWeek], time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond, time.wYear, carType, carId, event);
    }
    LeaveCriticalSection(&logCriticalSection);
}

DWORD WINAPI carThread(LPVOID param) {
    srand(GetCurrentThreadId());
    CarData* carData = (CarData*)param;
    int carId = carData->carId;
    int priority = carData->priority;

    // Arrive
    logEvent("Arrived at parking lot", carId, -1, priority);

    //Add car to proper priority queue
    EnterCriticalSection(&priorityCriticalSection);
    HANDLE threadHandle = GetCurrentThread();
    int size = queueSizes[priority];
    priorityQueues[priority][size] = threadHandle;
    queueSizes[priority]++;
    LeaveCriticalSection(&priorityCriticalSection);
    // Start counting time for parking
    clock_t start = clock();
    //Wait for the semaphore based in priority
    while (1) {
        EnterCriticalSection(&priorityCriticalSection);
        // Check if there are any cars with higher priority in other queues
        int higherPriorityExists = 0;
        for (int p = 0; p < priority; p++) { // Iterate through higher-priority queues
            if (queueSizes[p] > 0) {
                higherPriorityExists = 1;
                break;
            }
        }

    // If no higher-priority cars exist and this thread is at the front of its queue
        if (!higherPriorityExists && queueSizes[priority] > 0 && priorityQueues[priority][0] == threadHandle) {
            dequeue(priority); // Remove the thread from the queue
            LeaveCriticalSection(&priorityCriticalSection);

            // Wait for a parking spot
            DWORD waitResult = WaitForSingleObject(parkingSpotsSemaphore, 2000); // Acquire semaphore
            if(waitResult == WAIT_OBJECT_0){
                //Succesfully acquire the semaphore
                break;
            } else{
                // Failed to acquire the semaphore within the timeout, re-enter the queue
                EnterCriticalSection(&priorityCriticalSection);
                size = queueSizes[priority];
                priorityQueues[priority][size] = threadHandle;
                queueSizes[priority]++;
                LeaveCriticalSection(&priorityCriticalSection);
            }
        } else{
            LeaveCriticalSection(&priorityCriticalSection);
        }

        Sleep(100);
    }

    //End counting time for parking
    clock_t end = clock();
    float waiting_time = (float)(end - start) / CLOCKS_PER_SEC; // Convert to seconds
    
    // Park
    int parkingDuration = (rand() % 5) + 1;
    logEvent("Parked succesfully", carId, waiting_time, priority);

    // Add to average waiting time
    averageWaitingTime += waiting_time;
    parkedCars++;
    // Sleep for parking duration
    Sleep(parkingDuration * 1000);

    // Leave
    logEvent("Leaving", carId, -1, priority);
    ReleaseSemaphore(parkingSpotsSemaphore, 1, NULL);

    return 0;
}

int main() {
    // Initialize random seed
    srand((unsigned int)time(NULL));
    // Initialize semaphore and critical section
    parkingSpotsSemaphore = CreateSemaphore(NULL,   //Atributos de seguridad por defecto
        MAX_PARKING_SPOTS,                          //Valor inicial del semáforo
        MAX_PARKING_SPOTS,                          //Valor máximo del semáforo       
        NULL                                        //Nombre del semáforo (NULL = sin nombre)
    );
    InitializeCriticalSection(&logCriticalSection);
    InitializeCriticalSection(&priorityCriticalSection);

    HANDLE carThreads[NUM_CARS];
    CarData* carDataArray[NUM_CARS];

    // Create car threads
    for (int i = 0; i < NUM_CARS; i++) {
        carDataArray[i] = (CarData*)malloc(sizeof(CarData)); //Allocate memory for car data
        if(carDataArray[i] == NULL){
            printf("Error allocating memory\n");
            return 1;
        }
        carDataArray[i]->carId = i;
        float randomValue = (float)rand() / RAND_MAX; // Generate a random float between 0 and 1
        if (randomValue < 0.4) {
            carDataArray[i]->priority = 0; // 40% probability
        } else if (randomValue < 0.5) {
            carDataArray[i]->priority = 1; // 10% probability
        } else if (randomValue < 0.6) {
            carDataArray[i]->priority = 2; // 10% probability
        } else {
            carDataArray[i]->priority = 3; // 40% probability
        }
        carThreads[i] = CreateThread(NULL, 0, carThread, carDataArray[i], 0, NULL);
        if (carThreads[i] == NULL) {
            printf("Error creating thread\n");
            return 1;
        }
    }

    // Wait for all car threads to finish
    WaitForMultipleObjects(NUM_CARS, carThreads, TRUE, INFINITE);

    //Calculate average waiting time
    if(parkedCars > 0){
        averageWaitingTime /= parkedCars;
    }
    //Print last lines
    printf("Total cars parked: %d\n", parkedCars);
    printf("Average waiting time: %.2f seconds\n", averageWaitingTime);
    

    // Clean up
    for (int i = 0; i < NUM_CARS; i++) {
        CloseHandle(carThreads[i]); //Close thread handle
        free(carDataArray[i]);      //Free memory allocated for car data
    }
    CloseHandle(parkingSpotsSemaphore);
    DeleteCriticalSection(&logCriticalSection);
    DeleteCriticalSection(&priorityCriticalSection);

    return 0;
}