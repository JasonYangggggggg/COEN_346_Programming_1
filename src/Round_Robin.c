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
    // Create a wait queue
    struct Queue *waitQueue = initQueue();

    // Start timer
    int currentTime = 0;
    int nextRuntime = 0;

    struct Pcb currentPcb;
    struct Pcb previousPcb;

    char *rdyQueueContent = NULL;
    char *waitQueueContent = NULL;

    int n_processes = num_pid;
    printf("%d", n_processes);
    while (currentTime < exec_time) { // keep running until time fully elapsed
        // Check if there's a process in p_list that has arrived
        if (n_processes > 0) {
            for (int i = 0; i < sizeof(n_processes); i++) {
                if (p_list[i].arrival == currentTime) {
                    struct Pcb newPID = *createPcb(p_list[i].name, p_list[i].priority, p_list[i].burst,
                                                   p_list[i].arrival, p_list[i].num_children);
                    printf("New process: PID = %d, name = %s, prio = %d, burst = %d, arrival = %d\n", newPID.pid,
                           newPID.name, newPID.priority, newPID.burst, newPID.arrival);


                    addToQueue(readyQueue, newPID);
                    rdyQueueContent = printQueueContent(readyQueue);
                    // remove item from list
                    memmove(p_list + i, p_list + i + 1, (n_processes - i) * sizeof(*p_list));
                    // decrement num pid
                    n_processes -= 1;
                }
            }
        }

        // run new process iff there's no process running
        if (nextRuntime == currentTime) {
            if (previousPcb.pid != 0) {
                addToQueue(readyQueue, previousPcb);
            }
            // Grab first item from ready queue and execute it
            currentPcb = runPid(readyQueue);

            if (currentPcb.pid != -1) {
                rdyQueueContent = printQueueContent(readyQueue);
                waitQueueContent = printQueueContent(waitQueue);

                int execDuration = quantum; // default duration is quantum
                // check remaining burst of current pcb
                if (currentPcb.burst < quantum) {
                    execDuration = currentPcb.burst;
                }
                // check for children during execDuration. If
                if (currentPcb.num_children > 0) {
                    for (int i = 0; i < currentPcb.num_children; i++) {
                        if (currentPcb.children[i] != NULL & currentPcb.children[i]->arrival < nextRuntime) {
                            addToQueue(readyQueue, p_list[i]);

                        }
                    }
                }
                int burst = executePcb(&currentPcb, execDuration);
                nextRuntime = nextRuntime + burst;

                previousPcb = currentPcb;


            }
        }
        printf("%d\t\t%s\t\t%s\t\t%s\n", currentTime, currentPcb.name, rdyQueueContent, waitQueueContent);

        sleep(1);
        // increment currentTime
        currentTime += 1;
    }
    return 1;
}
