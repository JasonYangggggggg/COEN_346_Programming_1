#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/errno.h>

#define MIN_PID 300
#define MAX_PID 5000
#define MAXC 1024

FILE *input;

int main(int argc, char **argv) {
    char buffer[MAXC]; // buffer to read lines

    // I'm on Mac, so you may need to modify this line for windows
    input = argc > 1 ? fopen(argv[1], "r") : stdin;

    // Validate file open for reading
    if (!input) {
        perror("ERROR: Failed to open file");
        return 1;
    }

    int lines = 0; // init number of lines
    int c;

    while((c = fgetc(input)) != EOF) { // counts # of line breaks in file
        if ( c == '\n')
            ++ lines; // increment counter for lines if character encountered = line break
    }

    // reset pointer to SOF
    rewind(input);

    // init list of size lines
    int *p_list = malloc(lines * sizeof(int));

    while (fgets(buffer, MAXC, input)) {
        size_t len;
        buffer[(len = strcspn(buffer, "\n"))] = 0; // remove "\n" from line

    }
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
    pid_t pid;

    pid = fork();

    if (pid < 0) {
        printf("Error initializing pid");
        return -1;
    } else {
        return pid;
    }
}

// Releases a pid
void release_pid(int pid) {

}