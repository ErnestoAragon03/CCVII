#include "log_processor.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>




typedef struct {
    char url[MAX_LINE_LENGTH];
    int count;
} URLCount;

//Aqui son las variables que se compraten entre los hilos
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static int ip_count[HASH_SIZE] = {0};
static URLCount url_count[HASH_SIZE] = {0}; 
static int http_errors = 0; 

//200-299 → Éxito ✅
//300-399 → Redirección 🔄
//400-499 → Error del cliente ❌
//500-599 → Error del servidor ⚠️

// convertir una cadena en un índice en la tabla hash
static int hash_function(const char *str) {
    int hash = 0;
    while (*str) {
        hash = (hash * 31 + *str) % HASH_SIZE;  // Función de hash simple
        str++;
    }
    return hash;
}

//aqui analizamos la parte y la extraemos
void *process_log_chunk(void *arg) {
    ThreadData *data = (ThreadData *)arg;
    for (int i = data->start; i < (*data).end; i++) {
        char *line = (*data).lines[i];
        char ip[MAX_LINE_LENGTH], url[MAX_LINE_LENGTH];
        int status;
        if (sscanf(line, "%s - - [%*[^]]] \"%*s %s %d\"", ip, url, &status) == 3) {
            int ip_hash = hash_function(ip);
            int url_hash = hash_function(url);

            pthread_mutex_lock(&mutex);
            ip_count[ip_hash]++;

            // Almacenar URL correctamente
            if (url_count[url_hash].count == 0) {
                strncpy(url_count[url_hash].url, url, MAX_LINE_LENGTH);
            }
            url_count[url_hash].count++;

            if (status >= 400 && status < 600) {
                http_errors++;
            }
            pthread_mutex_unlock(&mutex);
        }
    }
    return NULL;
}

// Implementación vacía para evitar error que sin eso no sale bien el nomrbe dle output
void merge_results() {
   
}

void print_results() {
    int unique_ips = 0, max_visits = 0, max_url_index = -1;
    char most_visited_url[MAX_LINE_LENGTH] = "";

    //Aqui estamos guardando para ver de extra cual es la que mas se accede para ver la que mas se utiliza
    for (int i = 0; i < HASH_SIZE; i++) {
        if (ip_count[i] > 0) unique_ips++;
        if (url_count[i].count > max_visits) {
            max_visits = url_count[i].count;
            max_url_index = i;  // Guardamos el índice del hash
            strncpy(most_visited_url, url_count[i].url, MAX_LINE_LENGTH);
        }
    }

    printf("Total de IPs Únicas: %d\n", unique_ips);
    if (max_url_index != -1) {
        printf("URL Más Visitada: URL #%d (%d veces) %s\n", max_url_index, max_visits, most_visited_url);
    } else {
        printf("URL Más Visitada: No se encontraron URLs.\n");
    }
    printf("Errores HTTP: %d\n", http_errors);
}
