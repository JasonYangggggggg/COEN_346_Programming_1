//
// Created by Eve Gagnon on 2023-10-27.
//

#ifndef A1_READYQUEUE_H
#define A1_READYQUEUE_H
#include "Pcb.h"
#include <stddef.h>
#include <stdlib.h>

typedef struct Node {
    struct Node *next;
    struct Pcb data;
} Node;

struct ReadyQueue
{
    struct Node* front;
    struct Node* rear;
} ReadyQueue;

struct ReadyQueue* initQueue();

// Create node
struct Node* createNode(struct Pcb data);

// Add node to linked list
void addToQueue(struct ReadyQueue* queue, struct Pcb pcb);

// Run PID
struct Pcb runPid(struct ReadyQueue* queue);


#endif //A1_READYQUEUE_H
