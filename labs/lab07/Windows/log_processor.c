#include "log_processor.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>

#define MAX_IP_ADDRESS_LENGTH 16
#define MAX_URL_LENGTH 256
#define HASH_TABLE_SIZE 1000

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

    EnterCriticalSection(&ht->lock);  //Bloquear Sección Crítica
    
    Node* current = ht->table[index];
    while(current != NULL){
        if (strcmp(current->key, key)==0){
            current->value++;
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

void read_logs_chunk(FILE* archive, long start, long end, HashTable* ipTable, HashTable* urlTable, int* errorCount){
    char buffer[1024];
    //Mover puntero al inicio de chunk
    fseek(archive, start, SEEK_SET);
    //Leer todo el chunk
    while(ftell(archive) < end && fgets(buffer, sizeof(buffer), archive)){
        char ip[MAX_IP_ADDRESS_LENGTH], url[MAX_URL_LENGTH], accion[10];
        int status_code;
        //Extraer IP, URL y Status Code
        if(sscanf(buffer, "%s - - [%*[^]]] \"%s %[^\"]\" %d", ip, accion, url, &status_code) == 4){
            //Contar IP única
            insertOrUpdate(ipTable, ip);
            //Contar URL único
            insertOrUpdate(urlTable, url);
            //Contar errores
            if(status_code>=400 && status_code<=599){
                EnterCriticalSection(&ipTable->lock); //Se usa la sección crítica de ipTable arbitrariamente para sincronizar los errores
                (*errorCount)++;
                LeaveCriticalSection(&ipTable->lock);
            }
        }
    }
}