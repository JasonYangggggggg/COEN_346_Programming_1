#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "../include/Pcb.h"
#include "../include/Pid_Manager.h"
#include "../include/Queue.h"
#include "../include/Round_Robin.h"
#include "../include/Prio_RR.h"
#include "../include/Fcfs.h"

#define MIN_PID 300
#define MAX_PID 5000

#define NPTRS 2

struct Pcb *p_list;
struct Pcb *children_list;

// total time of execution
int exec_time = 0;

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

    p_list = malloc(line_count * sizeof(struct Pcb));
    struct Pcb* parent = NULL;
    int n_children = 0;
    int num_parents = 0;
    int idx = 0;
    // Map lines to struct process
    for (int i = 0; i < 5; i++)
    {
        // params
        char *p_name;
        int p_priority;
        int p_burst;
        int p_arrival;
        int p_num_children;

        int index = 0;
        char *token = strtok(lines[i], ", ");
        while (token != NULL)
        {
            // this is where you can store each of these tokens in a line to the design parameters
            if (index == 0)
            {
                // Allocate memory for the name field in the p_list structure
                p_name = malloc(strlen(token) + 1);
                if (p_name) {
                    strcpy(p_name, token);
                }
            }
            else if (index == 1)
            {
                p_priority = (int)strtol(token, (char **)NULL, 10);
            }
            else if (index == 2)
            {
                p_burst = (int)strtol(token, (char **)NULL, 10);
                // add burst to exec_time
                exec_time = exec_time + (int)strtol(token, (char **)NULL, 10);
            }
            else if (index == 3)
            {
                p_arrival = (int)strtol(token, (char **)NULL, 10);
            }
            else if (index == 4)
            {
                // get number of children
                p_num_children = (int)strtol(token, (char **)NULL, 10);
            }
            // get next token
            token = strtok(NULL, ",");
            index++;
        }

        struct Pcb* newReadPcb = readPcb(p_name, p_priority, p_burst, p_arrival, p_num_children);

        // if n_children > 0, decrement
        if (n_children > 0) {
            // Find first empty slot
            printf("idx = %d\n", idx);
            parent->children[idx] = newReadPcb;
            n_children--;
            idx++;
        } else {
            printf("added to p_list");
            p_list[i] = *newReadPcb;
        }

        if (newReadPcb->num_children > 0) {
            num_parents++;
            parent = newReadPcb; // set parent as newReadPcb.
            n_children = newReadPcb->num_children; // Set number of children expected.
            idx = 0;
        }
    }

    // Compare algorithms and select chosen one
    printf("exec time: %d\n", exec_time);
    return line_count;
}

int main(int argc, char *argv[])
{
    char* input_file = argv[1];
    char* algorithm = argv[2];
    int quantum = (int)strtol(argv[3], (char **)NULL, 10);

    // Read input file & get exec time
    int num_pid = read_file();

    // Reallocate p_list
    struct Pcb* new_p_list = malloc(num_pid * sizeof(struct Pcb));
    int num_parents = 0;

    for (int i = 0; i < num_pid; i++) {
        if (p_list[i].name != NULL) {
            new_p_list[num_parents] = p_list[i];
            num_parents++;
        }
    }

    // Adjust the size of the new_p_list
    new_p_list = realloc(new_p_list, num_parents * sizeof(struct Pcb));
    free(p_list);

    // Allocate map on program launch
    allocate_map();

    // Select algorithm
    if (strcmp(algorithm, "rr") == 0) {
        executeRR(new_p_list, quantum, exec_time, num_parents);
    } else if (strcmp(algorithm, "prr") == 0) {
        executePRR(new_p_list, quantum, exec_time, num_parents);
    } else if (strcmp(algorithm, "fcfs") == 0) {
        executeFCFS(new_p_list, exec_time, num_parents);
    }

    return 0;
}