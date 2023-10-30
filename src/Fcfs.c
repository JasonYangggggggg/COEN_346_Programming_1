#include "../include/Fcfs.h"
#include "../include/Queue.h"
#include <stdio.h>
#include <string.h>

int executeFCFS(struct Pcb *p_list, int exec_time, int num_pid)
{
    // initalize Queue
    struct Queue *readyQueue = initQueue();
    int current_time = 0;

    printf("Time\tReady\tRunning\tWaiting\n");
    printf("===================================\n");

    for (int i = 0; i < num_pid; i++)
    {
        while (p_list[i].arrival > current_time)
        {
            printf("%d\t-  \t-  \t-  \n", current_time);
            current_time++;
        }
        addToQueue(readyQueue, p_list[i]);
        struct Pcb current_process = runPid(readyQueue);
        printf("%d\t", current_time);
        printf("%s\t-  \t-  \n", current_process.name);
        current_time += current_process.burst;
        printf("%d\t-  \t-  \t-  \n", current_time);
    }
    return 1;
}
