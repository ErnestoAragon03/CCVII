#ifndef WAITINGLIST_H
#define WAITINGLIST_H


#include "pcb.h"


typedef struct Node {
    PCB* pcb;
    struct Node* next;
} Node;

typedef struct {
    Node* front;
    Node* rear;
    int size;
} WaitingList;

#define MAX_WAITING_NODES 32

extern WaitingList* waiting_list;

WaitingList* createWaitingList();
int enqueue(PCB* process);
int isWaitingListEmpty(WaitingList* list);
PCB* dequeue();
void freeWaitingList(WaitingList* list);
void insertInOrder(WaitingList* list, PCB* newProcess);
void reorderWaitingList(WaitingList* list);

#endif