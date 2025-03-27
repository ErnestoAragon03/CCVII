    #include <stdio.h>
    #include <stdlib.h>
    #include <pthread.h>
    #include <semaphore.h>
    #include <unistd.h>
    #include <time.h>
    #include <string.h>
    #include <stdbool.h>
    #include <ctype.h>

    #define N 3           // Número de espacios de estacionamiento
    #define CARS 10       // Número total de carros (9 normales + 1 ambulancia)
    #define MAX_WIDTH 120  // Ancho máximo de la terminal
    #define PARKING_WIDTH 25 // Ancho de cada espacio de parqueo
    

    sem_t parking_semaphore;
    pthread_mutex_t display_mutex;
    pthread_mutex_t log_mutex;
    pthread_mutex_t priority_mutex;

    typedef struct {
        int id;
        int state;       // 0=llegando, 1=esperando, 2=estacionado, 3=saliendo
        int position;    // Posición horizontal
        int space;       // Espacio asignado (-1 si no tiene)
        time_t arrival_time;
        time_t park_time;
        time_t leave_time;
        double wait_time;
        char plate[8];   // Placa del carro
        bool is_ambulance; // Prioridad máxima
    } Car;

    Car cars[CARS];
    bool parking_spaces[N];
    char transaction_log[1000][100]; // Almacena todas las transacciones
    int log_count = 0;
    bool priority_requested = false;

    // Arte ASCII para carros normales
    const char* car_art[] = {
        "   ______",
        "  /|_||_\\`.__",
        " (   _    _ _\\",
        "=`-(_)--(_)-'",
        "   O     O"
    };

    // Arte ASCII para ambulancia
    const char* ambulance_art[] = {
        "   __ooo__",
        "  /|_||_\\`.__",
        " (  AMBULANCE )",
        "=`-(_)--(_)-'",
        "   O     O"
    };

    const char* car_colors[] = {
        "\033[31m", "\033[32m", "\033[33m", "\033[34m", "\033[35m", 
        "\033[36m", "\033[37m", "\033[91m", "\033[92m", "\033[93m"
    };

    void log_transaction(const char* message, int car_id, double wait_time) {
        time_t now;
        time(&now);
        char time_str[30];
        strftime(time_str, sizeof(time_str), "%a %b %d %H:%M:%S %Y", localtime(&now));
        
        pthread_mutex_lock(&log_mutex);
        
        char vehicle_type[15];  
        if (cars[car_id].is_ambulance) {
            strcpy(vehicle_type, "Ambulance");
        } else {
            strcpy(vehicle_type, "Car");
        }

        if (wait_time > 0) {
            snprintf(transaction_log[log_count], 100, "[%s] %s %d (%s): %s (Esperar %.2f seg)", 
                    time_str, vehicle_type, car_id, cars[car_id].plate, message, wait_time);
        } else {
            snprintf(transaction_log[log_count], 100, "[%s] %s %d (%s): %s", 
                    time_str, vehicle_type, car_id, cars[car_id].plate, message);
        }

        log_count++;
        pthread_mutex_unlock(&log_mutex);
    }

    void clear_screen() {
        printf("\033[H\033[J");
    }

    void draw_car(int pos, int car_id) {
        pthread_mutex_lock(&display_mutex);

        // Determinar el color y el arte del vehículo
        const char* color_code;
        const char** vehicle_art;

        if (cars[car_id].is_ambulance) {
            color_code = "\033[41m\033[37m"; // Rojo con texto blanco para ambulancia
            vehicle_art = ambulance_art;
        } else {
            color_code = car_colors[car_id % 10];
            vehicle_art = car_art;
        }

        // Aplicar color y dibujar el vehículo
        printf("%s", color_code);
        for (int i = 0; i < 5; i++) {
            printf("\033[%d;%dH%s", 10 + i, pos, vehicle_art[i]);
        }

        // Mostrar placa debajo del vehículo
        printf("\033[15;%dH%s", pos + 3, cars[car_id].plate);

        printf("\033[0m"); // Reset de color
        pthread_mutex_unlock(&display_mutex);
    }


    void draw_parking_lot() {
        pthread_mutex_lock(&display_mutex);
        
        // Dibujar entrada más grande
        printf("\033[5;5H╔══════════════╗");
        printf("\033[6;5H║   ENTRADA    ║");
        printf("\033[7;5H╚══════════════╝");
        
        // Dibujar espacios de estacionamiento con más separación
        for (int i = 0; i < N; i++) {
            int x = 30 + i * (PARKING_WIDTH + 5); // Más espacio entre parqueos
            
            // Marco del espacio de parqueo
            printf("\033[8;%dH┌───────────────────────┐", x);
            printf("\033[9;%dH│      Espacio %-2d      │", x, i+1);
            for (int j = 10; j <= 16; j++) {
                printf("\033[%d;%dH│                       │", j, x);
            }
            printf("\033[17;%dH└───────────────────────┘", x);
            
            // Estado del espacio
            if (parking_spaces[i]) {
                printf("\033[12;%dH%s    OCUPADO    \033[0m", x+5, "\033[41m\033[37m");
            } else {
                printf("\033[12;%dH%s  DISPONIBLE  \033[0m", x+5, "\033[42m\033[30m");
            }
        }
        
        // Información de estado en la parte inferior
        printf("\033[20;1H════════════════ ESTADO DEL ESTACIONAMIENTO ════════════════");
        printf("\033[22;1HCarros estacionados: ");
        for (int i = 0; i < CARS; i++) {
            if (cars[i].state == 2) {
                const char* color_code;
                if (cars[i].is_ambulance) {
                    color_code = "\033[41m\033[37m"; // Rojo con texto blanco para ambulancia
                } else {
                    color_code = car_colors[i % 10];
                }
                printf("%s%s\033[0m ", color_code, cars[i].plate);
            }
        }
        
        printf("\033[24;1HCarros esperando:    ");
        for (int i = 0; i < CARS; i++) {
            if (cars[i].state == 1) {
                const char* color_code;
                if (cars[i].is_ambulance) {
                    color_code = "\033[41m\033[37m"; // Rojo con texto blanco para ambulancia
                } else {
                    color_code = car_colors[i % 10];
                }
                printf("%s%s\033[0m ", color_code, cars[i].plate);
            }
        }
        
        pthread_mutex_unlock(&display_mutex);
    }

    void update_display() {
        clear_screen();
        draw_parking_lot();
        
        for (int i = 0; i < CARS; i++) {
            if (cars[i].state != -1) {
                draw_car(cars[i].position, i);
            }
        }
        
        fflush(stdout);
    }
    void generate_plate(char* plate, bool is_ambulance) {
        static int regular_count = 0;
        const char* regular_plates[] = {
            "3CPO", "R2D2", "BD-1", "YODA", "OBI-1", 
            "VADER", "MAUL", "FALCON", "LUKE-S", "K2SO"
        };
        
        if (is_ambulance) {
           
            strncpy(plate, "AMB-PARSERCS", 8);
        } else {
            if (regular_count < 10) {
               
                strncpy(plate, regular_plates[regular_count], 8);
                regular_count++;
            } else {
                
                strncpy(plate, "XYZ-000", 8);
            }
        }
        plate[7] = '\0';  
    }
    
    void* car_thread(void* arg) {
        int car_id = *(int*)arg;
        free(arg);
        
        // Verificar si es ambulancia y solicitar prioridad
        if (cars[car_id].is_ambulance) {
            pthread_mutex_lock(&priority_mutex);
            priority_requested = true;
            pthread_mutex_unlock(&priority_mutex);
            
            // Esperar a que se libere un espacio inmediatamente
            while (true) {
                pthread_mutex_lock(&priority_mutex);
                bool space_found = false;
                for (int i = 0; i < N; i++) {
                    if (!parking_spaces[i]) {
                        space_found = true;
                        break;
                    }
                }
                
                if (space_found) {
                    // Forzar la salida de un carro si es necesario
                    for (int i = 0; i < N; i++) {
                        if (parking_spaces[i]) {
                            // Buscar el carro en este espacio
                            for (int j = 0; j < CARS; j++) {
                                if (cars[j].space == i && cars[j].state == 2) {
                                    cars[j].state = 3; // Forzar salida
                                    log_transaction("Leaving for ambulance priority", j, 0);
                                    break;
                                }
                            }
                        }
                    }
                    pthread_mutex_unlock(&priority_mutex);
                    break;
                }
                pthread_mutex_unlock(&priority_mutex);
                usleep(100000);
            }
        }
        
        // Inicializar carro
        cars[car_id].position = 5;
        cars[car_id].space = -1;
        time(&cars[car_id].arrival_time);
        
        log_transaction("Arrived at parking lot", car_id, 0);
        
        // Mover hacia la entrada
        while (cars[car_id].position < 20) {
            cars[car_id].position++;
            update_display();
            if (cars[car_id].is_ambulance) {
                usleep(30000);
            } else {
                usleep(80000);
            }
        }
        
        cars[car_id].state = 1; // Esperando
        time_t wait_start;
        time(&wait_start);
        
        // Ambulancia tiene prioridad máxima
        if (cars[car_id].is_ambulance) {
            // Esperar a que se libere un espacio
            while (true) {
                bool space_available = false;
                for (int i = 0; i < N; i++) {
                    if (!parking_spaces[i]) {
                        space_available = true;
                        break;
                    }
                }
                if (space_available) break;
                usleep(100000);
            }
        } else {
            sem_wait(&parking_semaphore); // Esperar espacio para carros normales
        }
        
        time_t wait_end;
        time(&wait_end);
        cars[car_id].wait_time = difftime(wait_end, wait_start);
        
        // Asignar espacio
        for (int i = 0; i < N; i++) {
            if (!parking_spaces[i]) {
                parking_spaces[i] = true;
                cars[car_id].space = i;
                break;
            }
        }
        
        cars[car_id].state = 2; // Estacionado
        time(&cars[car_id].park_time);
        log_transaction("Parked successfully", car_id, cars[car_id].wait_time);
        
        // Mover al espacio asignado
        int target_pos = 35 + cars[car_id].space * (PARKING_WIDTH + 5);
        while (cars[car_id].position < target_pos) {
            cars[car_id].position++;
            update_display();
            if (cars[car_id].is_ambulance) {
                usleep(50000);
            } else {
                usleep(100000);
            }
        }
        
        // Estacionado por un tiempo (menos tiempo para ambulancia)
        int sleep_time;
        if (cars[car_id].is_ambulance) {
            sleep_time = rand() % 2 + 1;
        } else {
            sleep_time = rand() % 5 + 3;
        }
        sleep(sleep_time);
            
        // Liberar espacio
        parking_spaces[cars[car_id].space] = false;
        cars[car_id].state = 3; // Saliendo
        time(&cars[car_id].leave_time);
        log_transaction("Leaving parking lot", car_id, 0);
        
        // Mover hacia la salida
        while (cars[car_id].position < MAX_WIDTH) {
        cars[car_id].position++;
        update_display();

        if (cars[car_id].is_ambulance) {
            usleep(30000); 
        } else {
            usleep(60000);
        }
    }

        
        cars[car_id].state = -1; // Se fue
        update_display();
        
        if (!cars[car_id].is_ambulance) {
            sem_post(&parking_semaphore);
        } else {
            pthread_mutex_lock(&priority_mutex);
            priority_requested = false;
            pthread_mutex_unlock(&priority_mutex);
        }
        
        return NULL;
    }

    void print_final_report() {
        clear_screen();
        printf("════════════════ FINAL TRANSACTION REPORT ════════════════\n\n");
        
        // Mostrar todas las transacciones en orden
        for (int i = 0; i < log_count; i++) {
            printf("%s\n", transaction_log[i]);
        }
        
        // Calcular estadísticas
        int total_parked = 0;
        double total_wait = 0.0;
        int ambulances_parked = 0;
        
        for (int i = 0; i < CARS; i++) {
            if (cars[i].wait_time >= 0) { // Contar solo los que lograron estacionarse
                total_parked++;
                total_wait += cars[i].wait_time;
                if (cars[i].is_ambulance) {
                    ambulances_parked++;
                }
            }
        }
        
        printf("\n════════════════ FINAL STATISTICS ════════════════\n");
        printf("Total cars parked: %d\n", total_parked);
        printf("Ambulances attended: %d\n", ambulances_parked);
        printf("Average wait time: %.2f seconds\n", total_parked > 0 ? total_wait / total_parked : 0);
        printf("\nPresione Enter para salir...");
        getchar();
    }

    int main() {
        srand(time(NULL));
        
        // Inicializar semáforo y mutex
        sem_init(&parking_semaphore, 0, N);
        pthread_mutex_init(&display_mutex, NULL);
        pthread_mutex_init(&log_mutex, NULL);
        pthread_mutex_init(&priority_mutex, NULL);
        
        // Inicializar espacios
        for (int i = 0; i < N; i++) {
            parking_spaces[i] = false;
        }
        
        // Inicializar carros (9 normales + 1 ambulancia)
        for (int i = 0; i < CARS; i++) {
            cars[i].state = -1;
            cars[i].wait_time = -1.0;
            cars[i].is_ambulance = (i == CARS-1); // El último carro es la ambulancia
            generate_plate(cars[i].plate, cars[i].is_ambulance);
        }
        
        clear_screen();
        printf("════════════════ SIMULACIÓN DE ESTACIONAMIENTO INTELIGENTE ════════════════\n\n");
        printf("  Configuración:\n");
        printf("  - Espacios de parqueo: %d\n", N);
        printf("  - Carros simulados: %d (incluyendo 1 ambulancia)\n\n", CARS);
        printf("Presione Enter para comenzar...");
        getchar();
         
        // Crear hilos para los carros
        pthread_t threads[CARS];
        for (int i = 0; i < CARS; i++) {
            int* car_id = malloc(sizeof(int));
            *car_id = i;
            
            // Determinar el delay de llegada
            int delay;
            if (i == CARS - 1) {  // Para la ambulancia (último elemento)
                delay = rand() % (CARS * 500000); 
            } else {               // Para carros normales
                delay = rand() % 1500000 + 500000;
            }
            
            pthread_create(&threads[i], NULL, car_thread, car_id);
            usleep(delay);
        }
        
        // Esperar a que terminen todos los carros
        for (int i = 0; i < CARS; i++) {
            pthread_join(threads[i], NULL);
        }
        
        // Mostrar reporte final
        print_final_report();
        
        // Limpiar recursos
        sem_destroy(&parking_semaphore);
        pthread_mutex_destroy(&display_mutex);
        pthread_mutex_destroy(&log_mutex);
        pthread_mutex_destroy(&priority_mutex);
        
        return 0;
    }