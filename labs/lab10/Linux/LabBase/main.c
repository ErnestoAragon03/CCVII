#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>
#include <sys/time.h>

#define CAPACIDAD_MAX_PUENTE 4
#define TIEMPO_MAX_ESPERA 5
#define TIEMPO_MAX_CRUCE 3
#define UMBRAL_INANICION 7 // segundos para considerar inanición

typedef struct {
    int id;
    int direccion;
    struct timeval hora_llegada;
} Estudiante;

// Variables globales protegidas por mutex
pthread_mutex_t mutex_puente = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_puente = PTHREAD_COND_INITIALIZER;

int direccion_actual = -1;
int estudiantes_en_puente = 0;
int esperando[2] = {0, 0}; // [0] derecha, [1] izquierda

// Variables para estadísticas
float tiempo_espera_total = 0;
int tiempo_espera_maximo = 0;
int estudiantes_inanicion = 0;
int total_estudiantes = 0;

void accederPuente(int direccion, int id, struct timeval hora_llegada) {
    pthread_mutex_lock(&mutex_puente);
    esperando[direccion]++;
    
    struct timeval inicio_espera;
    gettimeofday(&inicio_espera, NULL);
    
    while (estudiantes_en_puente == CAPACIDAD_MAX_PUENTE || 
           (estudiantes_en_puente > 0 && direccion_actual != direccion)) {
        pthread_cond_wait(&cond_puente, &mutex_puente);
    }
    
    esperando[direccion]--;
    estudiantes_en_puente++;
    if (estudiantes_en_puente == 1) {
        direccion_actual = direccion;
    }
    
    // Calcular tiempo de espera
    struct timeval fin_espera;
    gettimeofday(&fin_espera, NULL);
    int segundos_espera = fin_espera.tv_sec - inicio_espera.tv_sec;
    
    // Actualizar estadísticas
    tiempo_espera_total += segundos_espera;
    total_estudiantes++;
    
    if (segundos_espera > tiempo_espera_maximo) {
        tiempo_espera_maximo = segundos_espera;
    }
    
    if (segundos_espera >= UMBRAL_INANICION) {
        estudiantes_inanicion++;
    }
    
    printf("Inge %02d cruza hacia %s (en puente: %d)\n", 
           id, direccion == 0 ? "Derecha" : "Izquierda", estudiantes_en_puente);
    
    pthread_mutex_unlock(&mutex_puente);
}

void salirPuente(int id) {
    pthread_mutex_lock(&mutex_puente);
    estudiantes_en_puente--;
    
    printf("Inge %02d sale del puente (en puente: %d)\n", id, estudiantes_en_puente);
    
    if (estudiantes_en_puente == 0) {
        pthread_cond_broadcast(&cond_puente);
    } else {
        pthread_cond_signal(&cond_puente);
    }
    
    pthread_mutex_unlock(&mutex_puente);
}

void* estudiante(void* arg) {
    Estudiante* e = (Estudiante*)arg;
    
    // Espera aleatoria antes de llegar (0-5 segundos)
    sleep(rand() % (TIEMPO_MAX_ESPERA + 1));
    
    // Registrar hora de llegada
    gettimeofday(&e->hora_llegada, NULL);
    printf("Inge %02d llega queriendo ir a %s\n", 
           e->id, e->direccion == 0 ? "Derecha" : "Izquierda");
    
    accederPuente(e->direccion, e->id, e->hora_llegada);
    
    // Tiempo de cruce (1-3 segundos)
    sleep(1 + rand() % TIEMPO_MAX_CRUCE);
    
    salirPuente(e->id);
    
    free(e);
    return NULL;
}

void imprimirEstadisticas() {
    printf("\nESTADÍSTICAS:\n");
    printf("  Tiempo promedio de espera: %.2fs\n", tiempo_espera_total / total_estudiantes);
    printf("  Tiempo máximo de espera: %ds\n", tiempo_espera_maximo);
    printf("  Estudiantes que sufrieron inanición: %d\n", estudiantes_inanicion);
}

int main() {
    srand(time(NULL));
    pthread_t estudiantes[10];
    int ids_estudiantes[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int direcciones[10] = {1, 1, 0, 0, 1, 0, 1, 1, 1, 0}; // Para reproducir el ejemplo
    
    // Crear hilos de estudiantes
    for (int i = 0; i < 10; i++) {
        Estudiante* e = malloc(sizeof(Estudiante));
        e->id = ids_estudiantes[i];
        e->direccion = direcciones[i];
        pthread_create(&estudiantes[i], NULL, estudiante, e);
    }
    
    // Esperar a que todos los estudiantes terminen
    for (int i = 0; i < 10; i++) {
        pthread_join(estudiantes[i], NULL);
    }
    
    // Mostrar estadísticas
    imprimirEstadisticas();
    
    return 0;
}