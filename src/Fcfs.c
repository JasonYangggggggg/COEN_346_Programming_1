#include "../include/Fcfs.h"
#include "../include/Queue.h"
#include <stdio.h>
#include <string.h>

int executeFCFS(struct Pcb *p_list, int exec_time, int num_pid)
{
    // initalize Queue
    struct Queue *readyQueue = initQueue();
    int current_time = 0;
    int index_p = 0; // this can keep track on the number of processes in the list
   //before you enter the algorithm, you need to sort the list base on the arrive time
   //simple bubble sort
   for(int i = 0; i < num_pid - 1; i++){
       for(int j = 0; j < num_pid -i - 1; j++){
       
            if(p_list[j].arrival > p_list[j+1].arrival){
                    struct Pcb temp = p_list[j];
                    p_list[j] = p_list[j+1];
                    p_list[j+1] = temp;

             }
       }
    }
    int total_time = 0;
    // calculate the total time needed to finish the task
 
   for(int i = 0; i < num_pid; i++){
     total_time = total_time + p_list[i].burst;
   }

    printf("Time\tReady\tRunning\tWaiting\n");

    while(1){
        // hanlde ready queue
       // then increment the index_p if the there are still processes to implement and smaller than the current Time add them to readyQueue
       while(index_p < num_pid && p_list[index_p].arrival <= current_time){
             addToQueue(readyQueue, p_list[index_p]);
             index_p++;
       }
        
        // now if the readyqueue is empty that means no process has arrived then keep increasing the current time
        if(readyQueue -> front == NULL && readyQueue -> rear == NULL){
          current_time++;
          printf("%d\t- \t- \t- \n", current_time);
        }
        else{
        // if its not empty then you run the processes in the readyQueue
        struct Pcb current_process = runPid(readyQueue);
        printf("%d\t", current_time);
        printf("%s\t-  \t-  \n", current_process.name);
        current_time = current_time + current_process.burst;
        printf("%d\t-  \t-  \t-  \n", current_time);
       }
       if(current_time == total_time){
       break;
     }
     
    }
    return 1;
}
