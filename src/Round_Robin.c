//
// Created by Eve Gagnon on 2023-10-15.
//
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "../include/Round_Robin.h"
#include "../include/ReadyQueue.h"


int executeRR(struct Pcb *p_list, int quantum, int exec_time, int num_pid){
    // Create a ready queue
    struct ReadyQueue *readyQueue = initQueue();

    // Start timer
    int currentTime = 0;
    int nextRuntime = 0;
    struct Pcb currentPcb;


    while (currentTime < exec_time) { // keep running until time fully elapsed
        // Check if there's a process in p_list that has arrived
        for (int i = 0; i < num_pid; i++) {
            if (p_list[i].arrival == currentTime) {
                // remove item from list
                memmove(p_list + i, p_list + i + 1, (--num_pid - i)*sizeof(*p_list));
                // decrement num pid
                num_pid -= 1;

                struct Pcb newPID = *createPcb(p_list[i].name, p_list[i].priority, p_list[i].burst, p_list[i].arrival, p_list[i].children);
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

                if (currentPcb.children > 0) {
                    int num_children = sizeof (&currentPcb.children);
                    for (int j = 0; j < sizeof (&currentPcb.children); j++) {
                        if ((currentTime < currentPcb.children[j].arrival) && (currentPcb.children[j].arrival < nextRuntime)) {
                            memmove(currentPcb.children + j, currentPcb.children + j + 1, (-- num_children - j)*sizeof(*currentPcb.children));

                            // Realloc
                        }
                    }
                }
            }
        }
        sleep(1);
        // increment currentTime
        currentTime += 1;
    }
    return 1;
}
