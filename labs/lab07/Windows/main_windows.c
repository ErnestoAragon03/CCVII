#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "log_processor.h"

typedef struct{
    char** lines;
    int start;
    int end;
    HashTable* ipTable;
    HashTable* urlTable;
    int* errorCount;
    CRITICAL_SECTION* errorLock;        //Lock para el contador de errores
} ThreadData;

DWORD WINAPI startThread(LPVOID lpParam){
    ThreadData* data = (ThreadData*)lpParam;
    read_logs_chunk(data->lines, data->start, data->end, data->ipTable, data->urlTable, data->errorCount, data->errorLock);
    return 0;
}

//Función para contar las IPs únicas dentro de una HashTable
int countUniqueIPs(HashTable* ht){
    int count = 0;
    for(int i=0; i < HASH_TABLE_SIZE;i++){
        Node* current = ht->table[i];
        while(current != NULL){
            count++;
            current = current->next;
        }
    }
    return count;
}

//Función para encontrar la URL más visitada dentro de una HashTable
void findMostVistedURL(HashTable* ht, char* mostVistedURL, int* maxVisits){
    *maxVisits = 0;
    for (int i=0; i < HASH_TABLE_SIZE; i++){
        Node* current = ht->table[i];
        while(current != NULL){
            if(current->value > *maxVisits){
                *maxVisits = current->value;
                strcpy(mostVistedURL, current->key);
            }
            current = current->next;    
        }
    }
}


int main(int argc, char* argv[]){
    //Checar argumentos
    if(argc!=2){
        if(argc<2) printf("No se ha ingresado un log file como argumento.");

        else if(argc>2) printf("Se han ingresado argumentos demas.");

        printf("\tUso: %s <archivo.log> \n", argv[0]);
    }
    const char* filename = argv[1];
    FILE* archive = fopen(filename, "r");
    if(!archive){
        printf("Error al abrir el archivo %s\n",filename);
        return 1;
    }
    //Leer todas las líneas en el archivo
    char** lines = NULL;
    int numLines = 0;
    char buffer[1024];
    while(fgets(buffer, sizeof(buffer), archive)){
        lines = (char**)realloc(lines, (numLines+1)* sizeof(char*));
        lines[numLines] = strdup(buffer);   //Almacenar copia de la línea
        numLines++;
    }
    //Cerrar file
    fclose(archive);

    //Definir la cantidad de Threads a crear y el tamaño de los chunks
    int numThreads = 10;
    long chunkSize = numLines / numThreads;

    //Crear HashTables y contador de errores
    HashTable ipTable, urlTable;
    initHash(&ipTable);
    initHash(&urlTable);
    int errorCount = 0;

    //Inicializar sección crítica
    CRITICAL_SECTION errorLock;
    InitializeCriticalSection(&errorLock);

    //Crear un array de Handles y de ThreadData para los Threads
    HANDLE threads[numThreads];
    ThreadData threadData[numThreads];

    //Crear Threads
    for(int i=0; i<numThreads; i++){
        int start = i*chunkSize;
        int end = (i == numThreads - 1) ? numLines : start + chunkSize;

        threadData[i].lines = lines;
        threadData[i].start = start;
        threadData[i].end = end;
        threadData[i].ipTable = &ipTable;
        threadData[i].urlTable = &urlTable;
        threadData[i].errorCount = &errorCount;
        threadData[i].errorLock = &errorLock;
        
        threads[i] = CreateThread(NULL, 0, startThread, &threadData[i], 0, NULL);
        if(!threads[i]){
            printf("Error al crear el thread\n");
            return 1;
        }
        start = end+2;
        //printf("start: %d\n", start);
    }
    //Esperar a que los threads terminen
    WaitForMultipleObjects(numThreads, threads, TRUE, 5000);

    //Variables para guarar URL mas visitada
    char mostVisitedURL[MAX_URL_LENGTH];
    int maxVisits;
    findMostVistedURL(&urlTable, mostVisitedURL, &maxVisits);
    //Desplegar resultados
    printf("Total Unique IPs: %d\n", countUniqueIPs(&ipTable));
    printf("Most Visited URL: %s (%d times)\n", mostVisitedURL, maxVisits);
    printf("HTTP Errors: %d\n", errorCount);

    //Liberar memoria ocupada por las Hash Tables
    freeHashTable(&ipTable);
    freeHashTable(&urlTable);

    //Liberar memoria de las líneas
    for (int i=0;i<numLines;i++){
        free(lines[i]);
    }
    free(lines);

    //Eliminar secciones críticas
    DeleteCriticalSection(&errorLock);

    return 0;

}