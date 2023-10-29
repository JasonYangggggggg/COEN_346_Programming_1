//
// Created by Eve Gagnon on 2023-10-15.
//
#include "../include/Pid_Manager.h"
#include <stdlib.h>
#include <printf.h>

#define MIN_PID 300
#define MAX_PID 5000

#define SIZE (MAX_PID-MIN_PID)

int pid_bitmap[SIZE];

int allocate_map() {
    for(int i = 0; i < SIZE; i++) {
        pid_bitmap[i] = 0; // Mark all as Available in Bitmap
    }
    printf("Process Bitmap initialized... \n");
    return 1;
}
int allocate_pid() {
    for(int i = 0; i < SIZE; i++) {
        if(pid_bitmap[i] == 0) { // just make sure that pid is available
            pid_bitmap[i] = 1; // Allocate a Pid to new process
            return i + MIN_PID;
        }
    }
    return 1;
}
void release_pid(int pid) {
    if(MIN_PID <= pid < MAX_PID) {
        pid_bitmap[pid-MIN_PID] = 0;
        printf("Process Released with PID : %d \n",pid);
    }
}

#define NPTRS 2