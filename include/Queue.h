//
// Created by Eve Gagnon on 2023-10-27.
//

#ifndef A1_QUEUE_H
#define A1_QUEUE_H
#include "Pcb.h"
#include <stddef.h>
#include <stdlib.h>

typedef struct Node {
    struct Node *next;
    struct Pcb data;
} Node;

struct Queue
{
    struct Node* front;
    struct Node* rear;
} ReadyQueue;

struct Queue* initQueue();

// Create node
struct Node* createNode(struct Pcb data);

int isQueueEmpty(struct Queue* queue);

// Add node to linked list
void addToQueue(struct Queue* queue, struct Pcb pcb);

// Run PID
struct Pcb returnPid(struct Queue* queue);

char *printQueueContent(struct Queue* queue);

int checkWaitingQueue(struct Queue* waitQueue, struct Queue* readyQueues[5]);

int decreaseNumChildren(struct Queue* queue, char* parentName);

#endif //A1_QUEUE_H
