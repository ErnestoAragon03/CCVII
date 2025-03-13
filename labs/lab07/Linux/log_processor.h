#ifndef LOG_PROCESSOR_H
#define LOG_PROCESSOR_H

#include <pthread.h>

#define MAX_LINE_LENGTH 512
#define MAX_THREADS 4
#define HASH_SIZE 1000


// Estructura para pasar datos a los hilos
typedef struct {
    char **lines;
    int start;
    int end;
} ThreadData;

void *process_log_chunk(void *arg);
void merge_results();
void print_results();

#endif
