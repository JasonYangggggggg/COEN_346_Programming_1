//
// Created by Eve Gagnon on 2023-10-15.
//

#include <stdlib.h>
#include <printf.h>
#include <string.h>
#include "../include/Pcb.h"
#include "../include/Pid_Manager.h"


struct Pcb* readPcb(char *name, int priority, int burst, int arrival, int num_children) {
    struct Pcb* newPcb = (struct Pcb*) malloc(sizeof(struct Pcb));

    if (newPcb == NULL) { // mem alloc errors
        return NULL;
    }

    newPcb->name = strdup(name);  // Make a copy of the name
    if (newPcb->name == NULL) {
        free(newPcb);
        return NULL; // Memory allocation error
    }

    newPcb->priority = priority;
    newPcb->burst = burst;
    newPcb->arrival = arrival;
    newPcb->num_children = num_children;

    if (num_children > 0) {
        newPcb->children = (struct Pcb**)malloc(num_children * sizeof(struct Pcb*));
        if (newPcb->children == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            exit(1);
        }

        // Initialize child pointers to NULL
        for (int i = 0; i < num_children; i++) {
            newPcb->children[i] = NULL;
        }
    } else {
        newPcb->children = NULL;
    }

    printf("%s has priority %d, cpu burst %d, arrival time %d and %d children.\n", newPcb->name, newPcb->priority, newPcb->burst, newPcb->arrival, newPcb->num_children);
    return newPcb;
}

struct Pcb* createPcb(char *name, int priority, int burst, int arrival, int num_children) {
    struct Pcb* newPcb = (struct Pcb*) malloc(sizeof(struct Pcb));

    if (newPcb == NULL) { // mem alloc errors
        return NULL;
    }
    int newPid = allocate_pid();
    printf("PID allocated: %d\n", newPid);
    // Allocate PID & set PID to PCB
    newPcb->pid = newPid;

    newPcb->name = strdup(name);  // Make a copy of the name
    if (newPcb->name == NULL) {
        free(newPcb);
        return NULL; // Memory allocation error
    }

    newPcb->priority = priority;
    newPcb->burst = burst;
    newPcb->arrival = arrival;
    newPcb->num_children = num_children;

    if (num_children > 0) {
        newPcb->children = (struct Pcb**)malloc(num_children * sizeof(struct Pcb*));
        if (newPcb->children == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            exit(1);
        }

        // Initialize child pointers
        for (int i = 0; i < num_children; i++) {
            newPcb->children[i] = NULL;
        }
    } else {
        newPcb->children = NULL;
    }

    printf("Process %s [ID=%d] with priority = %d, burst = %d and arrival = %d created.\n", newPcb->name, newPcb->pid, newPcb->priority, newPcb->burst, newPcb->arrival);
    return newPcb;
}

int executePcb(struct Pcb* pcb, int burst) {
    printf("Inside PCB execution\n");
    // if process is executing
    // compare exec_burst to pcb burst
    if (burst > pcb->burst || burst == pcb->burst) { // if pcb burst is smaller OR if burst is equal, burst will be completely done
        int pcb_burst = pcb->burst;
        pcb->burst = 0;
        release_pid(pcb->pid);
        printf("[PID = %d] Process %s finished finished its CPU burst.\n", pcb->pid, pcb->name);
        return pcb_burst; // return the time it'll take to complete exec
    } else {
        pcb->burst = pcb->burst - burst;
        printf("[PID = %d] Process %s has %d ms burst left.\n", pcb->pid, pcb->name, pcb->burst);
        return burst; // return time it'll take to complete exec
    }
}

