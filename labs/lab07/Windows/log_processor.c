#include "log_processor.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>

#define MAX_IP_ADDRESS_LENGTH 16
#define MAX_URL_LENGTH 256
#define HASH_TABLE_SIZE 6

//Función para inicializar la Hash Table
void initHash(HashTable* ht){
    for(int i=0; i<HASH_TABLE_SIZE; i++){
        ht->table[i] = NULL;
    }
    InitializeCriticalSection(&ht->lock);   //Inicializar Sección Crítica
}

//Función Hash simple
unsigned int hash(const char* key){
    unsigned int hash_value = 0;
    for(int i =0; key[i] != '\0'; i++){
        hash_value = hash_value * 31 + key[i];
    }
    return hash_value % HASH_TABLE_SIZE;
}

//Insertar o actualizar en la Hash Table
void insertOrUpdate(HashTable* ht, const char* key){
    unsigned int index = hash(key);
    //printf("Key: %s\n", key);

    EnterCriticalSection(&ht->lock);  //Bloquear Sección Crítica
    
    Node* current = ht->table[index];
    while(current != NULL){
        if (strcmp(current->key, key)==0){
            current->value++;
            //printf("Current: %d\n", current->value);
            LeaveCriticalSection(&ht->lock);    //Desbloquear Sección Crítica
            return;
        }
        current = current->next;
    }

    //Si la key no existe, insertar nuevo nodo
    Node* new_node = (Node*)malloc(sizeof(Node));
    strcpy(new_node->key, key);
    new_node->value = 1;
    new_node->next = ht->table[index];
    ht->table[index] = new_node;

    LeaveCriticalSection(&ht->lock); //Desbloquear Sección Crítica
}

// Función para liberar la memoria de la Hash Table
void freeHashTable(HashTable *ht) {
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        Node *current = ht->table[i];
        while (current != NULL) {
            Node *temp = current;
            current = current->next;
            free(temp);  // Liberar cada nodo
        }
        ht->table[i] = NULL;  // Asegurarse de que el puntero sea NULL
    }
    DeleteCriticalSection(&ht->lock);  // Eliminar la sección crítica
}


// Función para procesar un conjunto de líneas
void read_logs_chunk(char **lines, int inicio, int fin, HashTable *ipTable, HashTable *urlTable, int *errorCount, CRITICAL_SECTION *errorLock) {
    for (int i = inicio; i < fin; i++) {
        char ip[MAX_IP_LENGTH], url[MAX_URL_LENGTH], accion[10];
        int status_code;

        // Extraer IP, URL y Status Code
        if (sscanf(lines[i], "%s - - [%*[^]]] \"%s %[^\"]\" %d", ip, accion, url, &status_code) == 4) {
            // Contar IP única
            insertOrUpdate(ipTable, ip);

            // Contar URL única
            insertOrUpdate(urlTable, url);

            // Contar errores
            if (status_code >= 400 && status_code <= 599) {
                EnterCriticalSection(errorLock);  // Bloquear la sección crítica para el contador de errores
                (*errorCount)++;
                LeaveCriticalSection(errorLock);  // Desbloquear la sección crítica
            }
        }
    }
}