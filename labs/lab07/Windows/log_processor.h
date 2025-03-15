#ifndef LOG_PROCESSOR_H
#define LOG_PROCESSOR_H

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_IP_LENGTH 16
#define MAX_URL_LENGTH 256
#define HASH_TABLE_SIZE 6

typedef struct Node{
    char key[MAX_URL_LENGTH];
    int value;
    struct Node *next;
} Node;

typedef struct {
    Node* table[HASH_TABLE_SIZE];
    CRITICAL_SECTION lock;  //Sección crítica para sincronización
} HashTable;

void initHash(HashTable* ht);
void insertOrUpdate(HashTable* ht, const char* key);
void freeHashTable(HashTable *ht);
void read_logs_chunk(char **lineas, int inicio, int fin, HashTable *ipTable, HashTable *urlTable, int *errorCount, CRITICAL_SECTION *errorLock);

#endif