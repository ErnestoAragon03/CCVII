#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>

#define PARQUEOS 3  
#define AUTOS 10 

sem_t semaforo_estacionamiento; // Semáforo para los PARQUEOS
pthread_mutex_t mutex_registro; // Mutex para el registro seguro
pthread_mutex_t mutex_estadisticas; // Mutex para estadísticas

int total_autos_estacionados = 0;
double tiempo_total_espera = 0.0;

void registrar_evento(const char *mensaje, int id_auto, double tiempo_espera) {
    pthread_mutex_lock(&mutex_registro);

    time_t ahora;
    time(&ahora);
    printf("[%s Auto %d: %s (esperó %.2f segundos)\n", ctime(&ahora), id_auto, mensaje, tiempo_espera);
    pthread_mutex_unlock(&mutex_registro);
}

void *auto_func(void *arg) {
    int id_auto = *(int *)arg;
    free(arg);
    
    time_t hora_llegada;
    time(&hora_llegada);
    registrar_evento("Llegó al estacionamiento", id_auto, 0.0);

    time_t inicio_espera;
    time(&inicio_espera);
    sem_wait(&semaforo_estacionamiento);
    
    time_t fin_espera;
    time(&fin_espera);
    double tiempo_espera = difftime(fin_espera, inicio_espera);
    registrar_evento("Se estacionó exitosamente", id_auto, tiempo_espera);

    pthread_mutex_lock(&mutex_estadisticas);
    total_autos_estacionados++;
    tiempo_total_espera += tiempo_espera;
    pthread_mutex_unlock(&mutex_estadisticas);

    sleep(rand() % 5 + 1); // Simular el tiempo estacionado (1-5 segs)
    registrar_evento("Salió del estacionamiento", id_auto, 0.0);
    sem_post(&semaforo_estacionamiento);
    
    return NULL;
}

int main() {
    srand(time(NULL));
    pthread_t autos[AUTOS];
    sem_init(&semaforo_estacionamiento, 0, PARQUEOS);
    pthread_mutex_init(&mutex_registro, NULL);
    pthread_mutex_init(&mutex_estadisticas, NULL);

    for (int i = 0; i < AUTOS; i++) {
        int *id_auto = malloc(sizeof(int));
        *id_auto = i;
        pthread_create(&autos[i], NULL, auto_func, id_auto);
        usleep(rand() % 500000); // Simular llegada aleatoria
    }

    for (int i = 0; i < AUTOS; i++) {
        pthread_join(autos[i], NULL);
    }

    sem_destroy(&semaforo_estacionamiento);
    pthread_mutex_destroy(&mutex_registro);
    pthread_mutex_destroy(&mutex_estadisticas);

    printf("\nTotal de autos estacionados: %d\n", total_autos_estacionados);
    printf("Tiempo promedio de espera: %.2f segundos\n", total_autos_estacionados > 0 ? tiempo_total_espera / total_autos_estacionados : 0);
    return 0;
}