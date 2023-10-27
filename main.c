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

int last;

struct process *rdy_q;
struct process *wait_q;
struct process *p_list;

int *pid_map;

// QUEUE DATA STRUCTURE
typedef struct node {
    struct process p;
    struct node *next;
} node_t;

void enqueue(node_t **head, struct process p) {
    node_t *new_node = malloc(sizeof (node_t));
    if (!new_node) return;

    new_node->p = p;
    new_node->next = *head;

    *head = new_node;
}

int dequeue(node_t **head) {
    node_t *current, *prev = NULL;
    int retval = -1;

    if (*head == NULL) return -1;

    current = *head;
    while (current->next != NULL) {
        prev = current;
        current = current->next;
    }

    retval = current->p;
    free(current);

    if (prev)
        prev->next = NULL;
    else
        *head = NULL;

    return retval;
}

/*char select_algo(struct process *process_list) {

}*/

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
    // init ready queue & wait list
    rdy_q = malloc(line_count*sizeof(struct process));
    wait_q = malloc(line_count*sizeof(struct process));

    // Compare algorithms and select chosen one
    printf("%d\n", (int) line_count);
    return (int) line_count;
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

void to_ready_q(struct process p) {
    // set pid of process that was just allocated
    p.pid = allocate_pid();

    // add process to first empty slot in ready queue
    for (int i = 0; i < sizeof (&rdy_q); i++) {
        if (!rdy_q[i].pid) {
            rdy_q[i] = p; // set pid
        }
    }

    printf("Process %d [pid = %d] with priority %d, CPU burst %d and %d children added to ready queue", p.name, p.pid, p.priority, p.CPU_Burst, p.n);
}

void fcfs_sched(struct process *process_list) {
    // if p.n > 0
    // every time the counter ++, then check if child arrives.
    //      -> if it arrives, add child to ready queue
    //      -> T1 goes into wait queue when ALL of its children have arrived
}

void rr_sched(int quantum, u_long total_time) {
    /*
     * At each time, looks in process list & checks if a process is arriving at current time. If there is, add p to ready queue
     * Concurrently, time quantum are executing. At the end of a time quantum, check for next p in rdy queue.
    */
    unsigned long time = 0;
    unsigned long tick = 1;
    int quantum_t = 0;

    struct process p;


    while (time < total_time) {
        // at each time, check if a process has arrived
        for (int i = 0; i < sizeof(&p_list); i++) {
            if (p_list[i].arrive_time == time) {
                // allocate pid
                to_ready_q(p_list[i]);
            }
        }

        /*
         * separate int to keep track of how many seconds it's been since the time quantum has started.
         * if elapsed < quantum, then skip UNLESS the process is done executing.
         * */
        // if rdy_q is not empty AND we are at a time quantum UNLESS
        if ((rdy_q != NULL) & (quantum_t % quantum == 0)) {
            // grab first process in rdy queue for execution
            p = rdy_q[0];
            // remove p from the rdy queue
            rdy_q[0].CPU_Burst -= quantum;

            // if process is done executing its burst time, then set quantum_t to 0
            if (rdy_q[0]){}
        }

        // increment time & quantum_t by 1
        time += tick;
        quantum_t += 1;
    }
}

void prr_sched(struct process *process_list, int quantum) {

}

/*void scheduler(char *algorithm, int *quantum, u_long total_time){
    unsigned long time = 0;
    unsigned long tick = 1;


    while (time < total_time) {
        // at each time, check if a process has arrived
        for (int i = 0; i < sizeof(&p_list); i++) {
            if (p_list[i].arrive_time == time) {
                // allocate pid
                int pid = allocate_pid();
                // add to ready queue
                to_ready_q(pid);
            }
        }

        // FCFS
        if (strcmp(algorithm, "fcfs") != 0) {


        } else if (strcmp(algorithm, "rr") != 0) {

        } else if (strcmp(algorithm, "rr") != 0) {

        }

        // increment time by 1
        time += tick;
    }
}*/


int main(int argc, char *argv[])
{
    char* input_file = argv[1];
    char* algorithm = argv[2];
    int quantum = atoi(argv[3]);

    read_file();
    // program init
    allocate_map();
    allocate_pid();

    printf("Time \t Running \t Waiting\n");
    /* */

    // FCFS
    if (strcmp(algorithm, "fcfs") != 0) {


    }
    // RR
    else if (strcmp(algorithm, "rr") != 0) {

    }
    // Priority RR
    else if (strcmp(algorithm, "prr") != 0) {

    }
    return 0;
}