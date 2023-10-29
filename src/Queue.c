//
// Created by Eve Gagnon on 2023-10-27.
//

#include <printf.h>
#include <string.h>
#include "../include/Queue.h"

// Initialize ready queue
struct Queue* initQueue()
{
    struct Queue* queue = (struct Queue*)malloc(sizeof(struct Queue));
    if (queue == NULL) {
        // Handle memory allocation error
        printf("ERROR");
        return NULL;
    }
    queue->front = queue->rear = NULL;
    printf("Ready queue initialized\n");
    return queue;
}

// Add node to linked list
void addToQueue(struct Queue* queue, struct Pcb data) {

    // Create node
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    if (newNode == NULL) {
        // Handle memory allocation error
        return;
    }

    newNode->data = data;
    newNode->next = NULL;

    if (queue->rear == NULL) {
        queue->front = queue->rear = newNode;
    } else {
        queue->rear->next = newNode;
        queue->rear = newNode;
    }

    printf("Process %s [pid = %d] added to Ready Queue\n", newNode->data.name, newNode->data.pid);
}

// Pop and return node
struct Pcb runPid(struct Queue* queue)
{
    // if Queue is empty, return pid.
    struct Pcb emptyPcb;
    emptyPcb.pid = -1;  // Sentinel value

    if (queue->front == NULL) {
        printf("ERROR: Queue is empty");
        return emptyPcb; // Queue is empty
    }

    struct Node* node = queue->front;
    queue->front = node->next;

    if (queue->front == NULL) {
        queue->rear = NULL;
    }

    struct Pcb data = node->data;
    printf("Process %s [pid = %d] is moved from Ready Queue to Running\n", data.name, data.pid);
    free(node);

    return data;
}

// Print all items in the queue
char *printQueueContent(struct Queue* queue) {
    struct Node* current = queue->front;

    char* names = NULL;
    size_t totalLength = 0;

    while (current != NULL) {
        size_t nameLength = strlen(current->data.name);
        names = realloc(names, totalLength + nameLength + 2); // +2 for separator and null terminator
        if (names == NULL) {
            perror("Memory allocation error");
            exit(EXIT_FAILURE);
        }

        if (totalLength > 0) {
            strcat(names, ", ");
            totalLength += 2;
        }
        strcat(names, current->data.name);
        totalLength += nameLength;

        current = current->next;
    }

    if (names == NULL) {
        names = "";
    }


    return names;
}