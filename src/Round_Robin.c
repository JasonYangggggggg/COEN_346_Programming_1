//
// Created by Eve Gagnon on 2023-10-15.
//
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "../include/Round_Robin.h"
#include "../include/Queue.h"


int executeRR(struct Pcb *p_list, int quantum, int exec_time, int num_pid){
    // Create a ready queue
    struct Queue *readyQueue = initQueue();

    // Start timer
    int currentTime = 0;
    int nextRuntime = 0;
    struct Pcb currentPcb;


    while (currentTime < exec_time) { // keep running until time fully elapsed
        // Check if there's a process in p_list that has arrived
        for (int i = 0; i < num_pid; i++) {
            if (p_list[i].arrival == currentTime) {
                struct Pcb newPID = *createPcb(p_list[i].name, p_list[i].priority, p_list[i].burst, p_list[i].arrival, p_list[i].children);
                printf("New process: PID = %d, name = %s, prio = %d, burst = %d, arrival = %d\n", newPID.pid, newPID.name,newPID.priority,newPID.burst,newPID.arrival);
                addToQueue(readyQueue, newPID);

                // remove item from list
                memmove(p_list + i, p_list + i + 1, (--num_pid - i)*sizeof(*p_list));
                // decrement num pid
                num_pid -= 1;
            }
        }

        // run new process iff there's no process running
        if (nextRuntime == currentTime) {
            // Grab first item from ready queue and execute it
            currentPcb = runPid(readyQueue);

            if (currentPcb.pid != -1) {
                // Exec process
                int burst = executePcb(&currentPcb, quantum);
                // set next running time
                nextRuntime = nextRuntime + burst;
                // if burst remaining, add back to ready queue.
                if (burst > 0) {
                    addToQueue(&Queue, currentPcb);
                }

                if (currentPcb.children > 0) {
                    for (int j = 0; j < currentPcb.children; j++) {
                        // add children of current pcb -> ready queue while process is running
                        if (p_list[j].arrival < nextRuntime) { // children has arrival time < nextRuntime
                            struct Pcb newChild = *createPcb(p_list[j].name, p_list[j].priority, p_list[j].burst, p_list[j].arrival, p_list[j].children);
                            addToQueue(readyQueue, newChild);

                            // remove item from list
                            memmove(p_list + j, p_list + j + 1, (--num_pid - j)*sizeof(*p_list));
                            // decrement num pid
                            num_pid -= 1;
                        }
                    }

                }
            }
        }
        printf("Time | Running | Ready | Waiting\n");
        printf("%d   %s   %s   \n", currentTime, currentPcb.name, (char *) readyQueue);
        sleep(1);
        // increment currentTime
        currentTime += 1;
    }
    return 1;
}
