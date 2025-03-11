#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "log_processor.h"

typedef struct{
    FILE* archive;
    long start;
    long end;
    HashTable* ipTable;
    HashTable* urlTable;
    int* errorCount;
} ThreadData;

DWORD WINAPI startThread(LPVOID lpParam){
    ThreadData* data = (ThreadData*)lpParam;
    read_logs_chunk(data->archive, data->start, data->end, data->ipTable, data->urlTable, data->errorCount);
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
        if(argc<2)printf("No se ha ingresado un log file como argumento.");

        else if(argc>2) printf("Se han ingresado argumentos demas.");

        printf("\tUso: %s <archivo.log> \n", argv[0]);
    }
    const char* filename = argv[1];
    FILE* archive = fopen(filename, "r");
    if(!archive){
        printf("Error al abrir el archivo %s\n",filename);
        return 1;
    }
    //Obtener tamaño del archivo
    fseek(archive, 0, SEEK_END);
    long fileSize = ftell(archive);
    fseek(archive, 0, SEEK_SET);

    //Definir la cantidad de Threads a crear y el tamaño de los chunks
    int numThreads = 1;
    int chunkSize = fileSize / numThreads;

    //Crear HashTables y contador de errores
    HashTable ipTable, urlTable;
    initHash(&ipTable);
    initHash(&urlTable);
    int errorCount = 0;

    //Crear un array de Handles y de ThreadData para los Threads
    HANDLE threads[numThreads];
    ThreadData threadData[numThreads];

    //Crear Threads
    for(int i=0; i<numThreads; i++){
        threadData[i].archive = archive;
        threadData[i].start = chunkSize*i;
        threadData[i].end = (i == numThreads-1) ? fileSize : chunkSize*(i+1);
        threadData[i].ipTable = &ipTable;
        threadData[i].urlTable = &urlTable;
        threadData[i].errorCount = &errorCount;
        
        threads[i] = CreateThread(NULL, 0, startThread, &threadData[i], 0, NULL);
        if(!threads[i]){
            printf("Error al crear el thread\n");
            return 1;
        }
    }
    //Esperar a que los threads terminen
    WaitForMultipleObjects(numThreads, threads, TRUE, INFINITE);
    
    fclose(archive);

    //Variables para guarar URL mas visitada
    char mostVisitedURL[MAX_URL_LENGTH];
    int maxVisits;
    findMostVistedURL(&urlTable, mostVisitedURL, &maxVisits);
    //Desplegar resultados
    printf("Total Unique IPs: %d\n", countUniqueIPs(&ipTable));
    printf("Most Visited URL: %s (%d times)\n", mostVisitedURL, maxVisits);
    printf("HTTP Errors: %d\n", errorCount);

    return 0;

}