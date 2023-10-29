//
// Created by Eve Gagnon on 2023-10-15.
//

#ifndef A1_PCB_H
#define A1_PCB_H

struct Pcb
{
    int pid;
    char *name;
    int priority;
    int burst;
    int arrival;
    int children;
} Pcb;

struct Pcb* createPcb(char *name, int priority, int burst, int arrival, int children);

int executePcb(struct Pcb* pcb, int burst);

#endif //A1_PCB_H
