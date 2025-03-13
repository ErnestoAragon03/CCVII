#include "log_processor.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define MAX_LINES 10000

char *log_lines[MAX_LINES];
int total_lines = 0;

void read_log_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error al abrir el archivo de log");
        exit(EXIT_FAILURE);
    }
    char buffer[MAX_LINE_LENGTH];
    while (fgets(buffer, sizeof(buffer), file) && total_lines < MAX_LINES) {
        log_lines[total_lines] = strdup(buffer);
        total_lines++;
    }
    fclose(file);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Uso: %s <archivo_log>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    read_log_file(argv[1]);

    pthread_t threads[MAX_THREADS];
    ThreadData thread_data[MAX_THREADS];
    int chunk_size = total_lines / MAX_THREADS;

    for (int i = 0; i < MAX_THREADS; i++) {
        thread_data[i].lines = log_lines;
        thread_data[i].start = i * chunk_size;
        thread_data[i].end = (i == MAX_THREADS - 1) ? total_lines : (i + 1) * chunk_size;
        pthread_create(&threads[i], NULL, process_log_chunk, &thread_data[i]);
    }

    for (int i = 0; i < MAX_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    merge_results();
    print_results();

    return 0;
}
