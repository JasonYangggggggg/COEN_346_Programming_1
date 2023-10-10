#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/errno.h>

#define MIN_PID 300
#define MAX_PId 5000

int main() {
    printf("Hello, World!\n");
    return 0;
}

// Creates and initializes a data structure for representing pids
//      -> Returns 1 if successful -1 if unsuccessful
//      -> Select data structure
int allocate_map(void) {
    // read input.txt & split it into n number of processes.
    //      -> Split each line into 5 diff fields
    //      -> Check last field for presence of children
    // how do i decide which algo to select??
    // -> Check it for the name too to know the orde of children.
}

// Allocates and returns a pid
//      -> Returns -1 if unable to allocate a process (all currently in use)
int allocate_pid(void) {

}

// Releases a pid
void release_pid(int pid) {

}