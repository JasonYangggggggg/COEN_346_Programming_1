//
// Created by Eve Gagnon on 2023-10-15.
//

#include <stdlib.h>
#include <printf.h>
#include "../include/Pcb.h"
#include "../include/Pid_Manager.h"

struct Pcb* createPcb(char *name, int priority, int burst, int arrival, int children) {
    struct Pcb* newPcb = (struct Pcb*) malloc(sizeof(struct Pcb));

    if (newPcb == NULL) { // mem alloc errors
        return NULL;
    }
    int newPid = allocate_pid();
    printf("PID allocated: %d\n", newPid);
    // Allocate PID & set PID to PCB
    newPcb->pid = newPid;
    newPcb->name = name;
    newPcb->priority = priority;
    newPcb->burst = burst;
    newPcb->arrival = arrival;
    newPcb->children = children;

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

