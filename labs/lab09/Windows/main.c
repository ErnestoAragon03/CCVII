#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h> // _kbhit() and _getch()

#define MAX_PARKING_SPOTS 5
#define NUM_CARS 10

// Struct to store the data of the car
typedef struct {
    int carId;
    int priority;
    int parked; //1 if parked, 0 if not
} CarData;

// Global variables

HANDLE carThreads[NUM_CARS];
CarData* carDataArray[NUM_CARS];

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
//CarData cars[NUM_CARS];

//GUI
int running = 1;
int parkingSpots[MAX_PARKING_SPOTS] = {0};
CRITICAL_SECTION consoleCriticalSection;

// Function to clear the console
void clearConsole() {
    system("cls");
}

// Function to set console text color
void setColor(int priority) {
    switch (priority) {
        case 0: printf("\033[31m"); break; // Red for Ambulance
        case 1: printf("\033[34m"); break; // Blue for Police
        case 2: printf("\033[33m"); break; // Yellow for Firefighter
        case 3: printf("\033[32m"); break; // Green for Regular Car
        default: printf("\033[0m"); break; // Reset color
    }
}

// Function to draw the parking lot and cars
void drawParkingLot() {
    EnterCriticalSection(&consoleCriticalSection);

    clearConsole();

    printf("Parking Lot Simulation\n");
    printf("=======================\n\n");

    // Draw parking spots
    for (int i = 0; i < MAX_PARKING_SPOTS; i++) {
        if (parkingSpots[i] == 0) {
            printf("[ EMPTY ] ");
        } else {
            int carId = parkingSpots[i];
            setColor(carDataArray[carId]->priority);
            printf("[ID %02d] ", carId);
            printf("\033[0m"); // Reset color
        }
    }
    printf("\n\n");

    // Draw cars waiting
    printf("Cars Waiting:\n");
    for (int i = 0; i < NUM_CARS; i++) {
        if (!carDataArray[i]->parked) {
            setColor(carDataArray[i]->priority);
            char* type;
            switch(carDataArray[i]->priority){
                case 0:
                    type = "Ambulance";
                    break;
                case 1:
                    type = "Police";
                    break;
                case 2: 
                    type= "Firefighter";
                    break;
                case 3:
                    type = "Car";
                    break;
            };
            printf("%s %d (Priority %d)\n",type, carDataArray[i]->carId, carDataArray[i]->priority);
            printf("\033[0m"); // Reset color
        }
    }

    LeaveCriticalSection(&consoleCriticalSection);
}


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
    EnterCriticalSection(&consoleCriticalSection);
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
    LeaveCriticalSection(&consoleCriticalSection);
}

DWORD WINAPI carThread(LPVOID param) {
    srand(GetCurrentThreadId());
    CarData* carData = (CarData*)param;
    int carId = carData->carId;
    int priority = carData->priority;

    // Arrive
    Sleep((rand() % 5 + 1) * 1000); //Random time to arrive
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
    EnterCriticalSection(&consoleCriticalSection);
        for (int i = 0; i < MAX_PARKING_SPOTS; i++) {
            if (parkingSpots[i] == 0) { // Empty spot found
                parkingSpots[i] = carId;
                carData->parked = 1;
                LeaveCriticalSection(&consoleCriticalSection);

                drawParkingLot();
                int parkingDuration = (rand() % 5 + 1) * 1000;
                logEvent("Parked succesfully", carId, waiting_time, priority);
                // Add to average waiting time
                averageWaitingTime += waiting_time;
                parkedCars++;
                // Sleep for parking duration
                Sleep(parkingDuration);

                // Leave
                EnterCriticalSection(&consoleCriticalSection);
                parkingSpots[i] = 0;
                LeaveCriticalSection(&consoleCriticalSection);
                drawParkingLot();
                logEvent("Leaving", carId, -1, priority);
                ReleaseSemaphore(parkingSpotsSemaphore, 1, NULL); // Release semaphore
                return 0;
            }
        }

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
    InitializeCriticalSection(&consoleCriticalSection);

    // Create car threads
    for (int i = 0; i < NUM_CARS; i++) {
        carDataArray[i] = (CarData*)malloc(sizeof(CarData)); //Allocate memory for car data
        if(carDataArray[i] == NULL){
            printf("Error allocating memory\n");
            return 1;
        }
        carDataArray[i]->carId = i;
        carDataArray[i]->parked = 0;
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
    DeleteCriticalSection(&consoleCriticalSection);
    DeleteCriticalSection(&priorityCriticalSection);
    DeleteCriticalSection(&logCriticalSection);

    return 0;
}