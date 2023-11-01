//
// Created by Eve Gagnon on 2023-10-29.
//

#include <printf.h>
#include <unistd.h>
#include <string.h>
#include "../include/Prio_RR.h"
#include "../include/Queue.h"

/*
 * Priority Round-Robin will add items to a queue at each time, but unlike the regular RR, the prio one will add
 * items to the ready queue according to their priority.
 * */
int executePRR(struct Pcb *p_list, int quantum, int exec_time, int num_pid) {
    // Create a ready queue/level of priority
    struct Queue *readyQueues[5];
    for (int i = 0; i < 5; i++) {
        readyQueues[i] = initQueue();
    }
    // Create a wait queue
    struct Queue *waitQueue = initQueue();

    // Start timer
    int currentTime = 0;
    int nextRuntime = 0;

    // State variables
    int currentPriority = 0;
    int priorityLevels = 5;
    int switchQueues = 0;
    int previousQueueIndex = 0;
    int currentQueueIndex = 0;
    int burstExec = 0; // number of times current process has executed yet.
    int interrupt = 0; // interrupt flag
    struct Pcb currentPcb;


    char *rdyQueueContent[5];
    char *waitQueueContent;

    int n_processes = num_pid;

    // Print template:
    printf("Time\t|\tRunning  \t\t|\tReady (Priority = 1)\t|Ready (Priority = 2)\t|Ready (Priority = 3)\t|Ready (Priority = 4)"
           "\t|Ready (Priority = 5)\t|\t\tWaiting\n");

    while (currentTime < exec_time) { // keep running until time fully elapsed
        // Check if there's a process in p_list that has arrived
        if (n_processes > 0) {
            int num_to_rdy_queue = 0;
            for (int i = 0; i < n_processes; i++) {
                if (p_list[i].arrival == currentTime) {
                    struct Pcb newPID = *createPcb(p_list[i].name, NULL, p_list[i].priority, p_list[i].burst,
                                                   p_list[i].arrival, p_list[i].num_children);
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
                    addToQueue(readyQueues[newPID.priority - 1], newPID);
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


        /*
         * Execute process in order of priority (1 -> 5)
         * While queue of prio 1 is NOT done, RR items in there.
         * If item of prio 1 arrives while queue of prio 3 is executing, the process stops executing and switches back to
         * the queue of higher priority.
         * */


        if (nextRuntime == currentTime) {

            // check wait queue
            int waitQupdate = checkWaitingQueue(waitQueue, readyQueues);
            if (waitQupdate == 1) {
                currentPriority = 0;
            }

            // At EACH time, we check for state of queue to find the first that is not empty.
            previousQueueIndex = currentQueueIndex; // set previousQueueIndex to the one that executed right before.
            int foundNonEmptyQueue = 0;  // A flag to indicate if a non-empty queue is found
            while (foundNonEmptyQueue == 0) {
                if (isQueueEmpty(readyQueues[currentPriority])) {
                    currentPriority = (currentPriority + 1) % priorityLevels;
                } else { // queue is NOT empty
                    currentQueueIndex = currentPriority;
                    foundNonEmptyQueue = 1;  // Set the flag to exit the loop
                }
            }


            /* If currentQueueIndex != previousQueueIndex, it means that the queue executing at the previous time needs to
             * be moved to the waiting queue. IFF
             * */
            if ((!isQueueEmpty(readyQueues[previousQueueIndex])) & (currentQueueIndex != previousQueueIndex)) {
                // Move previous ready queue to wait queue
                for (int i = 0; i < sizeof(&readyQueues[previousQueueIndex]); i++) {
                    addToQueue(waitQueue, returnPid(readyQueues[previousQueueIndex]));
                    switchQueues = 1;
                }
            } else {
                switchQueues = 0;
            }
            waitQueueContent = printQueueContent(waitQueue);
            currentPcb = returnPid(readyQueues[currentPriority]);

            if (currentPcb.pid != -1) {
                int execDuration = quantum; // default duration is quantum
                // check remaining burst of current pcb
                if (switchQueues == 1) {
                    execDuration = 1;
                } else if (currentPcb.burst < quantum) {
                    execDuration = currentPcb.burst;
                }

                // check for children during execDuration. If
                if (currentPcb.num_children > 0) {
                    int children_counter = 0;
                    for (int i = 0; i < currentPcb.num_children; i++) {
                        printf("current time = %d, exec duration = %d\n", currentTime, execDuration);
                        if (currentPcb.children[i] != NULL &
                            currentPcb.children[i]->arrival < (currentTime + execDuration)) {
                            struct Pcb *newChildPID = createPcb(currentPcb.children[i]->name,
                                                                currentPcb.name,
                                                                currentPcb.children[i]->priority,
                                                                currentPcb.children[i]->burst,
                                                                currentPcb.children[i]->arrival,
                                                                currentPcb.children[i]->num_children);

                            addToQueue(readyQueues[newChildPID->priority - 1], *newChildPID);
                            printf("Added children %s from parent %s to queue %d\n", newChildPID->name, newChildPID->parent, (newChildPID->priority - 1));
                            children_counter++;

                            //free(currentPcb.children[i]);
                            printf("children count: %d\n", children_counter);
                            // if counter == num_children, find the time at which the last child arrived and set execDuration
                            if (children_counter == currentPcb.num_children) {
                                printf("inside children counter\n");
                                if (currentPcb.children[i]->arrival < currentTime) {
                                    execDuration = 1;
                                } else {
                                    execDuration = currentPcb.children[i]->arrival - currentTime + 1;
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
                    for (int i = 0; i < 5; i++) {
                        rdyQueueContent[i] = printQueueContent(readyQueues[i]);
                    }
                } else {
                    // add to rdy queue if burst > 0
                    if (currentPcb.burst > 0) {
                        for (int i = 0; i < 5; i++) {
                            rdyQueueContent[i] = printQueueContent(readyQueues[i]);
                        }
                        addToQueue(readyQueues[currentPriority], currentPcb);
                    } else {
                        // process is done
                        // check if it has a parent and if so, update counter
                        if (currentPcb.parent) {
                            decreaseNumChildren(waitQueue, currentPcb.parent);
                        }
                    }
                }
            }
        }
        printf("%d\t\t%s\t\t%s\t\t%s\t\t%s\t\t%s\t\t%s\t\t%s\n", currentTime, currentPcb.name, rdyQueueContent[0], rdyQueueContent[1], rdyQueueContent[2], rdyQueueContent[3], rdyQueueContent[4], waitQueueContent);

        sleep(1);
        // increment currentTime
        currentTime += 1;
    }
    return 1;
}
