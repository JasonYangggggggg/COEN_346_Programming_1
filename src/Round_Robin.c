//
// Created by Eve Gagnon on 2023-10-15.
//

#include <sys/types.h>
#include <printf.h>
#include "../include/Round_Robin.h"
#include "../include/ReadyQueue.h"

struct Pcb executeRR(struct Pcb *p_list, int quantum, int exec_time){
    int pid_quantity = sizeof (&p_list);
    // Create a ready queue
    struct ReadyQueue *readyQueue = initQueue();

    // Start timer
    int currentTime = 0;
    int nextRuntime = 0;
    struct Pcb currentPcb;

    while (currentTime < exec_time) { // keep running until time fully elapsed
        // Check if there's a process in p_list that has arrived
        for (int i = 0; i < pid_quantity; i++) {
            printf("Process %s [ID = %d] with arrival = %d, burst = %d", p_list[i].name, p_list[i].pid, p_list[i].arrival, p_list[i].burst);
            if (p_list[i].arrival == currentTime) {
                struct Pcb newPID = *createPcb(*p_list[i].name, p_list[i].priority, p_list[i].burst, p_list[i].arrival, p_list[i].children);
                addToQueue(readyQueue, newPID);
            }
        }

        // run new process iff there's no process running
        if (nextRuntime == currentTime) {
            // Grab first item from ready queue and execute it
            currentPcb = runPid(readyQueue);

            if (currentPcb.pid != -1) {
                // Exec RR
                int burst = executePcb(&currentPcb, quantum);
                // set next running time
                nextRuntime = nextRuntime + burst;
            }
        }

        // increment currentTime
        currentTime += 1;
    }
}
