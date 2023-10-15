#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define MIN_PID 300
#define MAX_PID 5000

#define NPTRS 2

struct process
{
    int pid;
    char name;
    int priority;
    int CPU_Burst;
    int arrive_time;
    int n;
    char status;
};



int *pid_bitmap;

int process_count;
int last;

struct process *ready_q;
struct process *wait_q;
struct process *p_list;

int *pid_map;

char select_algo(struct process *process_list) {

}

int read_file(void)
{
    char line[150];
    char **lines = NULL;
    size_t  nptrs = NPTRS, line_count = 0;

    FILE *file = fopen("../input.txt", "r");
    if (!file)
    {
        perror("Error opening the file");
        return 1;
    }

    if ((lines = malloc (nptrs * sizeof *lines)) == NULL) {
        perror ("malloc-lines");
        exit (EXIT_FAILURE);
    }

    while (fgets(line, sizeof(line), file))
    {
        size_t length;
        line[(length = strcspn (line, "\n"))] = 0;

        if (line_count == nptrs) {
            void *tmp = realloc (lines, (2 * nptrs) * sizeof *lines);
            if (!tmp) {
                perror ("realloc-lines");
                break;
            }
            lines = tmp;
            nptrs *= 2;
        }

        if (!(lines[line_count] = malloc (length + 1))) {
            perror ("malloc-lines[used]");
            break;
        }
        memcpy (lines[line_count], line, length + 1);

        line_count += 1;
    }

    if (file != stdin) {
        fclose(file);
    }

    p_list = malloc(line_count * sizeof(struct process));
    // Map lines to struct process
    int index = 0;
    for (int i = 0; i < line_count; i++)
    {
        char *token = strtok(lines[i], ",");
        while (token != NULL)
        {
            // this is where you can store each of these tokens in a line to the design parameters
            printf("Token: %s\n", token);
            if (index == 0)
            {
                p_list[i].name = token[0];
            }
            else if (index == 1)
            {
                p_list[i].priority = atoi(token);
            }
            else if (index == 2)
            {
                p_list[i].CPU_Burst = atoi(token);
            }
            else if (index == 3)
            {
                p_list[i].arrive_time = atoi(token);
            }
            else if (index == 4)
            {
                p_list[i].n = atoi(token);
            }

            // get next token
            token = strtok(NULL, ",");
            index++;
        }

        index = 0;
        printf("**********************");
        putchar('\n');
    }

    // Compare algorithms and select chosen one
    printf("%d\n", line_count);
    return line_count;
}

void to_ready_q(int pid) {
    printf("Send pid %d to ready queue", pid);
    if (pid_map) {
        // find first empty spot
        for (int i = 0; i < process_count; i++) {
            if (!pid_map[i]) {
                pid_map[i] = pid;
            }
        }
    }
}


int allocate_map(void) {
    pid_bitmap = malloc((MAX_PID - MIN_PID + 1) * sizeof (int));
    if (pid_bitmap) {
        int i;
        for (i = 0; i < (MAX_PID - MIN_PID + 1); i++) {
            pid_bitmap[i] = 0; // set all bits in array to 0
        }
    } else {
        return -1;
    }

    // init pid map
    pid_map = malloc(sizeof(int[process_count][2]));

    return 1;
}

int allocate_pid(void)
{
    int counter = 0; // count # of
    int next; // pointer to the next pid

    // loops back to min_pid
    if (last + 300 == MAX_PID) {
        next = MIN_PID - 300;
    } else {
        next = last + 1;
    }

    while ((counter <= MAX_PID-MIN_PID) && (pid_bitmap[next] == 1)) {
        ++counter;
        ++next;
        if (next > MAX_PID - 300) {
            next = MIN_PID - 300;
        }
    }

    // if counter has reached 4701 without allocating process, then it's full
    if (counter == MAX_PID-MIN_PID + 1) {
        printf("error");
        return -1;
    }
    pid_bitmap[next] = 1;
    last = next;
    int pid = last + 300;

    return pid;
}

 // Releases a pid
void release_pid(int pid)
{
    pid_bitmap[pid-300] = 0;
}

void fcfs_sched(struct process p) {
    // if p.n > 0
    // every time the counter ++, then check if child arrives.
    //      -> if it arrives, add child to ready queue
    //      -> T1 goes into wait queue when ALL of its children have arrived
}

void sjf_sched(struct process p) {

}

void psjf_sched(struct process p) {

}

void scheduler(char **algorithm){
    clock_t start_t, end_t;
    double total_t;
    int i;
    
    // FCFS
    if (strcmp(algorithm, "fcfs")) {
        // at each time, check if a process is supposed to arrive
        for (int i = 0; i < sizeof (p_list); i++) {
            if (p_list[i].arrive_time == total_t) {
                allocate_pid();
            }
        }
    } else if (strcmp(algorithm, "sjf")) {

    } else if (strcmp(algorithm, "psjf")) {

    }
}


int main()
{
    read_file();
    // program init
    allocate_map();
    allocate_pid();

    printf("Time \t Running \t Waiting\n");
    /* */

    /*if (process_size == 0) {
        int testing = allocate_pid();
        printf("%d", testing);
    } else {
        printf("error");
        return 1;
    }*/
    return 0;
}