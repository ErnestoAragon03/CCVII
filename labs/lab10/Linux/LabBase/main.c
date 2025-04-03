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
#define UMBRAL_INANICION 7
#define NUM_ESTUDIANTES 10

typedef struct {
    int id;
    int direccion;
    struct timeval hora_llegada;
} Estudiante;

pthread_mutex_t mutex_puente = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_puente = PTHREAD_COND_INITIALIZER;

int direccion_actual = -1;
int estudiantes_en_puente = 0;
int esperando[2] = {0, 0}; // [0] derecha, [1] izquierda

struct timeval tiempo_espera_primero[2]; // tiempos de llegada del primero esperando por dirección

// Estadísticas
float tiempo_espera_total = 0;
int tiempo_espera_maximo = 0;
int estudiantes_inanicion = 0;
int total_estudiantes = 0;

int calcularDiferenciaSegundos(struct timeval inicio, struct timeval fin) {
    return (int)(fin.tv_sec - inicio.tv_sec);
}

void accederPuente(int direccion, int id, struct timeval hora_llegada) {
    pthread_mutex_lock(&mutex_puente);
    esperando[direccion]++;

    // Si es el primero en esa dirección, registramos su hora de llegada
    if (esperando[direccion] == 1) {
        tiempo_espera_primero[direccion] = hora_llegada;
    }

    while (estudiantes_en_puente == CAPACIDAD_MAX_PUENTE || 
           (estudiantes_en_puente > 0 && direccion_actual != direccion) ||
           (estudiantes_en_puente == 0 && direccion_actual != -1 &&
            calcularDiferenciaSegundos(tiempo_espera_primero[!direccion_actual], hora_llegada) >= UMBRAL_INANICION)) {
        
        // Forzamos cambio si hay inanición detectada en la dirección opuesta
        if (estudiantes_en_puente == 0 &&
            esperando[!direccion] > 0) {
            struct timeval ahora;
            gettimeofday(&ahora, NULL);
            int espera_opuesta = calcularDiferenciaSegundos(tiempo_espera_primero[!direccion], ahora);
            if (espera_opuesta >= UMBRAL_INANICION) {
                direccion_actual = !direccion;
                pthread_cond_broadcast(&cond_puente);
            }
        }

        pthread_cond_wait(&cond_puente, &mutex_puente);
    }

    esperando[direccion]--;
    estudiantes_en_puente++;

    if (estudiantes_en_puente == 1) {
        direccion_actual = direccion;
    }

    struct timeval ahora;
    gettimeofday(&ahora, NULL);
    int segundos_espera = calcularDiferenciaSegundos(hora_llegada, ahora);

    tiempo_espera_total += segundos_espera;
    total_estudiantes++;
    if (segundos_espera > tiempo_espera_maximo)
        tiempo_espera_maximo = segundos_espera;
    if (segundos_espera >= UMBRAL_INANICION)
        estudiantes_inanicion++;

    printf("Inge %02d cruza hacia %s (en puente: %d)\n", 
           id, direccion == 0 ? "Derecha" : "Izquierda", estudiantes_en_puente);

    pthread_mutex_unlock(&mutex_puente);
}

void salirPuente(int id) {
    pthread_mutex_lock(&mutex_puente);
    estudiantes_en_puente--;

    printf("Inge %02d sale del puente (en puente: %d)\n", id, estudiantes_en_puente);

    if (estudiantes_en_puente == 0) {
        direccion_actual = -1; // permitir cambio de dirección
        pthread_cond_broadcast(&cond_puente);
    } else {
        pthread_cond_signal(&cond_puente);
    }

    pthread_mutex_unlock(&mutex_puente);
}

void* estudiante(void* arg) {
    Estudiante* e = (Estudiante*)arg;

    sleep(rand() % (TIEMPO_MAX_ESPERA + 1));
    gettimeofday(&e->hora_llegada, NULL);

    printf("Inge %02d llega queriendo ir a %s\n", 
           e->id, e->direccion == 0 ? "Derecha" : "Izquierda");

    accederPuente(e->direccion, e->id, e->hora_llegada);

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
    pthread_t estudiantes[NUM_ESTUDIANTES];
    int ids_estudiantes[NUM_ESTUDIANTES] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int direcciones[NUM_ESTUDIANTES] = {1, 1, 0, 0, 1, 0, 1, 1, 1, 0}; // puedes randomizar si gustas

    for (int i = 0; i < NUM_ESTUDIANTES; i++) {
        Estudiante* e = malloc(sizeof(Estudiante));
        e->id = ids_estudiantes[i];
        e->direccion = direcciones[i];
        pthread_create(&estudiantes[i], NULL, estudiante, e);
    }

    for (int i = 0; i < NUM_ESTUDIANTES; i++) {
        pthread_join(estudiantes[i], NULL);
    }

    imprimirEstadisticas();
    return 0;
}
