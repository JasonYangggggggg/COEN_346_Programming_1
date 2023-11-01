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

    // flag Interrupt
    char interrupt = 0;

    struct Pcb currentPcb;
    struct Pcb previousPcb;

    char *rdyQueueContent = NULL;
    char *waitQueueContent = NULL;

    int n_processes = num_pid;

    // Print template:
    printf("Time\t|\tRunning  \t\t|\t\tReady\t\t|\t\tWaiting\n");

    // algorithm
    while (currentTime < exec_time) { // keep running until time fully elapsed
        // Check if there's a process in p_list that has arrived
        if (n_processes > 0) {
            int num_to_rdy_queue = 0;
            for (int i = 0; i < n_processes; i++) {
                printf("index i = %d\n", i);
                if (p_list[i].arrival == currentTime) {
                    struct Pcb newPID = *createPcb(p_list[i].name, NULL, p_list[i].priority, p_list[i].burst,
                                                   p_list[i].arrival, p_list[i].num_children);
                    printf("New process: PID = %d, name = %s, prio = %d, burst = %d, arrival = %d\n", newPID.pid,
                           newPID.name, newPID.priority, newPID.burst, newPID.arrival);
                    // If num children > 0, add children
                    if (newPID.num_children > 0) {

                        // sort p_list children
                        for (int j = 0; j < newPID.num_children; j++) {
                            for(int k = 0; k < newPID.num_children - j - 1; k++) {
                                if (p_list[i].children[k]->arrival > p_list[i].children[k + 1]->arrival) {
                                    struct Pcb temp = *p_list[i].children[k];
                                    p_list[i].children[k] = p_list[i].children[k + 1];
                                    p_list[i].children[k + 1] = &temp;
                                }
                            }
                        }
                        for (int j = 0; j < newPID.num_children; j++) {
                            printf("adding child %s with arrival time %d\n", p_list[i].children[j]->name, p_list[i].children[j]->arrival);
                            newPID.children[j] = p_list[i].children[j];
                        }
                    }


                    addToQueue(readyQueue, newPID);
                    num_to_rdy_queue++;
                }
            }
            if (num_to_rdy_queue > 0) {
                // remove n items from list
                memmove(p_list + num_to_rdy_queue, p_list + num_to_rdy_queue + 1, (n_processes - num_to_rdy_queue) * sizeof(*p_list));
                // decrement num pid
                n_processes -= num_to_rdy_queue;
            }
        }

        // run new process iff there's no process running
        if (nextRuntime == currentTime) {
            // check for waitQueueContent
            waitQueueContent = printQueueContent(waitQueue);
            // Grab first item from ready queue and execute it
            currentPcb = returnPid(readyQueue);

            if (currentPcb.pid != -1) {

                int execDuration = quantum; // default duration is quantum
                // check remaining burst of current pcb
                if (currentPcb.burst < quantum) {
                    execDuration = currentPcb.burst;
                }
                // check for children during execDuration. If
                if (currentPcb.num_children > 0) {
                    int children_counter = 0;
                    for (int i = 0; i < currentPcb.num_children; i++) {
                        if (currentPcb.children[i] != NULL & currentPcb.children[i]->arrival < (currentTime + execDuration)) {
                            struct Pcb *newChildPID = createPcb(currentPcb.children[i]->name,
                                                                currentPcb.name,
                                                                currentPcb.children[i]->priority,
                                                                currentPcb.children[i]->burst,
                                                                currentPcb.children[i]->arrival,
                                                                currentPcb.children[i]->num_children);
                            addToQueue(readyQueue, *newChildPID);
                            children_counter++;

                            free(currentPcb.children[i]);

                            // if counter == num_children, find the time at which the last child arrived and set execDuration
                            if (children_counter == currentPcb.num_children) {
                                if (currentPcb.children[i]->arrival < currentTime) {
                                    execDuration = 1;
                                } else {
                                    execDuration = currentPcb.children[i]->arrival - currentTime;
                                }
                                interrupt = 1; // set flag to 1
                            }
                        }
                    }
                    // Update the count of children
                    currentPcb.num_children -= children_counter;
                } else {
                    interrupt = 0;
                }
                int burst = executePcb(&currentPcb, execDuration);
                nextRuntime = nextRuntime + burst;

                // if flag = 1, send currentPcb to waitQueue
                if (interrupt == 1) {
                    addToQueue(waitQueue, currentPcb);

                    rdyQueueContent = printQueueContent(readyQueue);
                    printf("Rdy queue content after interrupt: %s\n", rdyQueueContent);
                } else {
                    // add to rdy queue if burst > 0
                    if (currentPcb.burst > 0) {
                        rdyQueueContent = printQueueContent(readyQueue);
                        addToQueue(readyQueue, currentPcb);
                    }
                }

            }

        }
        printf("%d\t\t%s\t\t%s\t\t%s\n", currentTime, currentPcb.name, rdyQueueContent, waitQueueContent);

        sleep(1);
        // increment currentTime
        currentTime += 1;
    }
    return 1;
}
