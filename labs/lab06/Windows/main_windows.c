#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

//Estructura para pasar datos a los hilos
typedef struct{
    char* file_path;    //Path del archivo
} ThreadData;

//Función para procesar los archivos (por hilo)
DWORD WINAPI process_file(LPVOID lpParam){
    ThreadData* data = (ThreadData *)lpParam;
    char* file_path = data->file_path;

    FILE *file = fopen(file_path, "r");
    if (file == NULL){
        fprintf(stderr,"[Thread ID: %lu] Error al abrir el archivo: %s\n", GetCurrentThreadId(), file_path);
        return 0;
    }
    /*Tiempo de espera para simular tiempo de
    Sleep(5000);
    */
    //Leer número de líneas
    int line_count = 0;
    char ch;
    while((ch = fgetc(file)) != EOF){
        if(ch == '\n'){
            line_count++;
        }
    }

    // Si el archivo no terminó en \n hay que contar la última línea
    fseek(file, -1, SEEK_END);
    if(fgetc(file) != '\n'){
        line_count++;
    }
    // Cerrar el archivo
    fclose(file);

    //Imprimir resultado
    printf("[Thread ID: %lu] %s has %d\n",GetCurrentThreadId(), file_path, line_count);

    return (DWORD)line_count;
}

int main(int argc, char* argv[]){
    if(argc < 2){
        fprintf(stderr, "Uso: %s <file1> <file2> ... <fileN>\n", argv[0]);
        return 1;
    }
    //Array para almacenar identificadores de los threads
    HANDLE* threads = (HANDLE*)malloc((argc-1)* sizeof(HANDLE));
    ThreadData* thread_data = (ThreadData*)malloc((argc-1)* sizeof(ThreadData));
    if(threads == NULL || thread_data == NULL){
        fprintf(stderr,"Error al asignar memoria para los threads\n");
        return 1;
    }
    //Crear Thread para cada archivo
    for(int i=1; i<argc; i++){
        thread_data[i-1].file_path = argv[i];   //Asignarle el PATH del archivo
        //Crear hilo
        threads[i-1] = CreateThread(
            NULL,               //Atributos de seguridad (NULL para default)
            0,                  //Tamaño de Stack (0 para tamaño default)
            process_file,       //Puntero a la función que ejecutará el thread
            &thread_data[i-1],  //Datos que se pasarán al thread
            0,                  //Banderas de creación (0 para que el hilo se ejecute inmediatamente despúes de la creación)
            NULL                //Variable que recibe el ID del thread (NULL para no guardarlo)
        );

        if(threads[i-1] == NULL){
            fprintf(stderr, "Error al crear un Thread para el arvhivo: %s\n", argv[i]);
            return 1;
        }
    }
    //Esperar para que los hilos terminen
    WaitForMultipleObjects(argc-1, threads, TRUE, INFINITE);

    /*  Forma alternativa de Thread Joining con WaitForSingleObject
    for (int i = 0; i < argc - 1; i++) {
        WaitForSingleObject(threads[i], INFINITE);
    }
    */

    //Sumar conteo de líneas en cada thread
    DWORD total_lines = 0;
    for (int i=0;i<argc-1;i++){
        DWORD exit_code;
        if(GetExitCodeThread(threads[i], &exit_code)){
            total_lines += exit_code;
        } else{
            fprintf(stderr,"Error durante el conteo de lineas totales: No se pudo obtener el codigo de salida del thread para el archivo %s\n",argv[i+1]);
        }
    }

    //Desplegar el conteo total de líneas
    printf("Total lines counted: %lu\n", total_lines);
    
    // Cerrar los identificadores de los hilos
    for(int i=0; i<argc-1;i++){
        CloseHandle(threads[i]);
    }
    //Liberar la memoria reservada con malloc
    free(threads);
    free(thread_data);
    return 0;
}