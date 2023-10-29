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
    // Map lines to struct process
    for (int i = 0; i < 5; i++)
    {
        int index = 0;
        char *token = strtok(lines[i], ", ");
        while (token != NULL)
        {
            // this is where you can store each of these tokens in a line to the design parameters
            if (index == 0)
            {
                printf("Name: %s\n", token);
                // Allocate memory for the name field in the p_list structure
                p_list[i].name = malloc(strlen(token) + 1);
                if (p_list[i].name) {
                    strcpy(p_list[i].name, token);
                }
            }
            else if (index == 1)
            {
                printf("Prio: %s\n", token);
                p_list[i].priority = (int)strtol(token, (char **)NULL, 10);
            }
            else if (index == 2)
            {
                printf("Burst: %s\n", token);
                p_list[i].burst = (int)strtol(token, (char **)NULL, 10);
                // add burst to exec_time
                printf("Exec before add: %d\n", exec_time);
                exec_time = exec_time + (int)strtol(token, (char **)NULL, 10);
                printf("Exec after add: %d\n", exec_time);
            }
            else if (index == 3)
            {
                printf("Arrival: %s\n", token);
                p_list[i].arrival = (int)strtol(token, (char **)NULL, 10);
            }
            else if (index == 4)
            {
                printf("n Children: %s\n", token);
                // get number of children
                //int n_children = (int)strtol(token, (char **)NULL, 10);
                p_list[i].children = (int)strtol(token, (char **)NULL, 10);
//                // alloc for children list
//                children_list = malloc(n_children * sizeof(struct Pcb));
//                // add children to the list
//                int c_index = 0;
//                for (int j = 0; j < n_children; j++) {
//                    i++;
//                    char *c_token = strtok(lines[i], ",");
//                    while (c_token != NULL) {
//                        if (c_index == 0) {
//                            children_list[i].name = &c_token[0];
//                        } else if (c_index == 1) {
//                            children_list[i].priority = (int)strtol(c_token, (char **)NULL, 10);
//                        } else if (c_index == 2) {
//                            children_list[i].burst = (int)strtol(c_token, (char **)NULL, 10);
//                            // add burst to exec_time
//                            exec_time = exec_time + (int)strtol(c_token, (char **)NULL, 10);
//                        } else if (c_index == 3) {
//                            children_list[i].arrival = (int)strtol(c_token, (char **)NULL, 10);
//                        }
//                        // get next token
//                        c_token = strtok(NULL, ",");
//                        c_index++;
//                    }
//                    c_index = 0;
//                }
//                p_list[i].children = malloc(n_children * sizeof(struct Pcb));
//                p_list[i].children = children_list;
            }


            // get next token
            token = strtok(NULL, ",");
            index++;
        }
        printf("Process %s has priority %d, cpu burst %d, arrival time %d and %d children.\n", p_list[i].name, p_list[i].priority, p_list[i].burst, p_list[i].arrival, p_list[i].children);
        printf("**********************\n");
    }

    // Compare algorithms and select chosen one
    printf("exec time: %d\n", exec_time);
    return (int) line_count;
}

int main(int argc, char *argv[])
{
    char* input_file = argv[1];
    char* algorithm = argv[2];
    int quantum = (int)strtol(argv[3], (char **)NULL, 10);

    // Read input file & get exec time
    int num_pid = read_file();

    // Allocate map on program launch
    allocate_map();

    // Select algorithm
    if (strcmp(algorithm, "rr") != 0) {
        executeRR(p_list, quantum, exec_time, num_pid);
    }

    return 0;
}