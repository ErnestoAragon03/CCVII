#include "waitingList.h"
#include "utils.h"


static WaitingList static_waiting_list; // Lista estática
WaitingList* waiting_list = &static_waiting_list;


static Node static_nodes[MAX_WAITING_NODES];
static int free_node_index = 0;

void reset_static_nodes() {
    free_node_index = 0;
    for (int i = 0; i < MAX_WAITING_NODES; i++) {
        static_nodes[i].pcb = NULL;
        static_nodes[i].next = NULL;
    }
}

//Create a new waiting list
WaitingList* createWaitingList() {
    static_waiting_list.front = NULL;
    static_waiting_list.rear = NULL;
    static_waiting_list.size = 0;
    reset_static_nodes(); // Resetear los nodos estáticos
    return &static_waiting_list;
}

//Enqueue a process to the waiting list
int enqueue(PCB* pcb) {
    if (free_node_index >= MAX_WAITING_NODES) {
        return -1; // No hay nodos libres
    }
    Node* newNode = &static_nodes[free_node_index++];
    newNode->pcb = pcb;
    newNode->next = NULL;

    if (waiting_list->rear) {
        waiting_list->rear->next = newNode;
    }
    waiting_list->rear = newNode;

    if (!waiting_list->front) {
        waiting_list->front = newNode;
    }

    waiting_list->size++;
    return 0; // Éxito
}

//Check if the waiting list is empty
int isWaitingListEmpty(WaitingList* list) {
    return list->size == 0;
}

PCB* dequeue() {
    if (isWaitingListEmpty(waiting_list)) {
        return NULL;
    }

    Node* temp = waiting_list->front;
    PCB* pcb = temp->pcb;
    waiting_list->front = waiting_list->front->next;

    if (!waiting_list->front) {
        waiting_list->rear = NULL;
    }

    // "Libera" el nodo (opcional: podrías implementar una pila de nodos libres)
    temp->pcb = NULL;
    temp->next = NULL;

    waiting_list->size--;

    // Decrementa el índice de nodos libres
    if (free_node_index > 0) {
        free_node_index--;
    }
    
    return pcb;
}