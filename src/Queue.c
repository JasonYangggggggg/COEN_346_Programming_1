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

// Check if queue is empty
int isQueueEmpty(struct Queue* queue) {
    return (queue->front == NULL);
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
struct Pcb returnPid(struct Queue* queue)
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
    } else {
        char *token = strtok(names, ", ");
        char unique[1000];  // Adjust the size accordingly
        char result[1000] = "";

        while (token) {
            if (strstr(unique, token) == NULL) {
                strcat(result, token);
                strcat(result, " ");
                strcat(unique, token);
                strcat(unique, ", ");
            }
            token = strtok(NULL, ", ");
        }

        if (result[strlen(result) - 1] == *" ") {
            result[strlen(result) - 1] = '\0';
        }
        strcpy(names, result);
    }

    return names;
}

struct Pcb peekFront(struct Queue* queue) {
    struct Pcb emptyPcb;
    emptyPcb.pid = -1; // Sentinel value

    if (queue->front == NULL) {
        printf("Queue is empty\n");
        return emptyPcb;
    }

    return queue->front->data;
}

// Check waiting queue and move processes to ready queues if all children are done
int checkWaitingQueue(struct Queue* waitQueue, struct Queue* readyQueues[5]) {
    struct Pcb firstPcb = peekFront(waitQueue);

    if (firstPcb.pid == -1) {
        // Queue is empty, nothing to check
        return 0;
    }
    if (firstPcb.children_done == 0) {
        // Move the front process back to its ready queue
        returnPid(waitQueue);
        addToQueue(readyQueues[firstPcb.priority - 1], firstPcb);
        printf("first pcb prio: %d\n", firstPcb.priority);
    }
    return 1;
}

int decreaseNumChildren(struct Queue* queue, char* parentName) {
    if (queue == NULL || parentName == NULL) {
        // Handle invalid queue or parent name
        return 0;
    }

    // Traverse the list to find the node with the specified parent name
    struct Node* current = queue->front;
    while (current != NULL) {
        if (strcmp(current->data.name, parentName) == 0) {
            // Node with the specified parent name is found
            // Modify the num_children field within this node
            if (current->data.children_done> 0) {
                current->data.children_done--;
            }
            printf("parent %s has %d left\n", parentName, current->data.children_done);
            return 1; // Modification successful
        }
        current = current->next;
    }

    // Node with the specified parent name not found
    return 0; // Node not found
}