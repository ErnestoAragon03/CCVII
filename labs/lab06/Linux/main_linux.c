#include <stdio.h>
#include <stdlib.h>
#include <pthread.h> //permite el uso de hilos

void *count_lines(void *arg) { //pthread_create requiere este tipo.
    char *filename = (char *)arg;
    FILE *file = fopen(filename, "r"); //abrir en modo lectura
    if (!file) {
        printf("[Thread ID: %lu] Error al abrir el archivo: %s\n", pthread_self(), filename);
        return (void *)-1;
    }

    long count = 0;  // Se usa long para que pueda convertirse a void*
    char buffer[1024];

    //AQui estamos contando cada linea
    while (fgets(buffer, sizeof(buffer), file)) {
        count++;
    }
    fclose(file); //aqui etamos liberando los recuros lo que hablamos en la calse
     
    printf("[Thread ID: %lu] %s tiene %ld líneas.\n", pthread_self(), filename, count);
    return (void *)count;  // Convertir el entero a void* 
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Uso: %s <archivo1> <archivo2> ...\n", argv[0]);
        return EXIT_FAILURE;
    }

    int num_files = argc - 1; //argv 0 es el run.sh 
    pthread_t threads[num_files]; //son la cantidad de archivos que se van analizar
    long total_lines = 0; 

    // Crear hilos para cada archivo
    for (int i = 0; i < num_files; i++) {
        pthread_create(&threads[i], NULL, count_lines, argv[i + 1]);
        //crea y ejecuta un nuevo hilo
    }

    // Esperar a que los hilos finalicen y acumular el total de líneas
    for (int i = 0; i < num_files; i++) {
        void *result;
        pthread_join(threads[i], &result);
        if (result != (void *)-1) {
            total_lines += (long)result;  // Convertir el puntero de vuelta a long
        }
    }

    printf("Total de líneas contadas: %ld\n", total_lines);
    return EXIT_SUCCESS;
}
