//
// Created by Eve Gagnon on 2023-10-29.
//

#include <printf.h>
#include <unistd.h>
#include <string.h>
#include "../include/Prio_RR.h"
#include "../include/Queue.h"

int executePRR(struct Pcb *p_list, int quantum, int exec_time, int num_pid) {
    // Create a ready queue/level of priority
    struct Queue *priorityReadyQueue[5];
    for (int i = 0; i < 5; i++) {
        priorityReadyQueue[i] = initQueue();
    }
    // Create a wait queue
    struct Queue *waitQueue = initQueue();

    // Start timer
    int currentTime = 0;
    int nextRuntime = 0;
    struct Pcb currentPcb;

    char *rdyQueueContent;
    char *waitQueueContent;

    int n_processes = num_pid;
    printf("%d", n_processes);
    while (currentTime < exec_time) { // keep running until time fully elapsed
        // Check if there's a process in p_list that has arrived
        if (n_processes > 0) {
            for (int i = 0; i < sizeof(n_processes); i++) {
                if (p_list[i].arrival == currentTime) {
                    struct Pcb newPID = *createPcb(p_list[i].name, p_list[i].priority, p_list[i].burst,
                                                   p_list[i].arrival, p_list[i].children);
                    printf("New process: PID = %d, name = %s, prio = %d, burst = %d, arrival = %d\n", newPID.pid,
                           newPID.name, newPID.priority, newPID.burst, newPID.arrival);
                    // grab priority of new process and add it to correct ready queue
                    int newPIDPriority = newPID.priority - 1;
                    addToQueue(priorityReadyQueue[newPIDPriority], newPID);
                    // remove item from list
                    memmove(p_list + i, p_list + i + 1, (n_processes - i) * sizeof(*p_list));
                    // decrement num pid
                    n_processes -= 1;
                }
            }
        }

        /*
         * Execute process in order of priority (1 -> 5)
         * While
         * */

        printf("%d   %s   %s   %s\n", currentTime, currentPcb.name, rdyQueueContent, waitQueueContent);

        sleep(1);
        // increment currentTime
        currentTime += 1;
    }
    return 1;
}
