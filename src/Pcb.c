//
// Created by Eve Gagnon on 2023-10-15.
//

#include <stdlib.h>
#include <printf.h>
#include "../include/Pcb.h"
#include "../include/Pid_Manager.h"

struct Pcb* createPcb(char *name, int priority, int burst, int arrival, struct Pcb* children) {
    struct Pcb* newPcb = (struct Pcb*) malloc(sizeof(struct Pcb));

    if (newPcb == NULL) { // mem alloc errors
        return NULL;
    }
    int newPid = allocate_pid();
    printf("%d", newPid);
    // Allocate PID & set PID to PCB
    newPcb->pid = allocate_pid();
    newPcb->name = name;
    newPcb->priority = priority;
    newPcb->burst = burst;
    newPcb->arrival = arrival;
    newPcb->children = children;

    return newPcb;
}

int executePcb(struct Pcb* pcb, int burst) {
    printf("Inside PCB execution\n");
    // if process is executing
    // compare exec_burst to pcb burst
    if (burst > pcb->burst) { // if pcb burst is smaller, burst will be completely done
        int pcb_burst = pcb->burst;
        pcb->burst = 0;
        return pcb_burst; // return the time it'll take to complete exec
    } else {
        pcb->burst = pcb->burst - burst;
        return burst; // return time it'll take to complete exec
    }
}

