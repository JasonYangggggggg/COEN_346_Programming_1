#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define MIN_PID 300
#define MAX_PID 5000

FILE *file;

int process_size = 0;
struct process
{
    char name;
    int priority;
    int CPU_Burst;
    int arrive_time;
    int n;
    int currently_inuse;
};
struct process *container;


int allocate_map(void)
{
    printf("%d\n", process_size);
    struct process object[process_size];

    file = fopen("../input.txt", "r");
    if (!file)
    {
        perror("Error opening the file");
        return 1;
    }
    char line[150];
    char *lines[30];
    int line_count = 0;
    while (fgets(line, sizeof(line), file) != NULL)
    {
        size_t length = strlen(line);
        if (line[length - 1] == '\n')
        {
            line[length - 1] = '\0';
        }

        lines[line_count] = strdup(line);
        line_count++;
    }

    fclose(file);

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
                object[i].name = token[0];
            }
            else if (index == 1)
            {
                object[i].priority = atoi(token);
            }
            else if (index == 2)
            {
                object[i].CPU_Burst = atoi(token);
            }
            else if (index == 3)
            {
                object[i].arrive_time = atoi(token);
            }
            else if (index == 4)
            {
                object[i].n = atoi(token);
            }

            // get next token
            token = strtok(NULL, ",");
            index++;
        }

        index = 0;
        printf("**********************");
        putchar('\n');
    }
    for (int i = 0; i < process_size; i++)
    {
        container[i] = object[i];
    }
    // testing if such items store in the object
    for (int i = 0; i < line_count; i++)
    {
        printf("%d\n", object[i].priority);
        printf("**********************");
        putchar('\n');
    }
    printf("done");
    return 0;
}

// not quite sure if this is the right way
int allocate_pid(void)
{
    static int current_pid = MIN_PID;

    // check if all the pid is in use (more than 5000)
    if (current_pid > MAX_PID)
    {
        return -1;
    }

    for (int i = 0; i < process_size; i++)
    {
        // object can be global value
        if (container[i].n == current_pid)
        {
            current_pid++;
            return allocate_pid();
        }
    }
    for (int i = 0; i < process_size; i++)
    {
        if (container[i].n == current_pid)
        {
            container[i].currently_inuse = 1;
        }
    }

    return current_pid++;
}

// // Releases a pid
void release_pid(int pid)
{
    for (int i = 0; i < process_size; i++)
    {
        if (container[i].n == pid)
        {
            container[i].currently_inuse = 0;
            break;
        }
    }
}


int main()
{
    /*FILE *file = fopen("input.txt", "r");
    if (file == NULL)
    {
        perror("Error opening the file");
        return 1;
    }
    char line[150];
    char *lines[30];
    int line_count = 0;
    while (fgets(line, sizeof(line), file) != NULL)
    {
        size_t length = strlen(line);
        if (line[length - 1] == '\n')
        {
            line[length - 1] = '\0';
        }

        lines[line_count] = strdup(line);
        line_count++;
    }

    fclose(file);

    process_size = line_count;
    container = malloc(process_size * sizeof(struct process));*/
    // call the function to create the map
    int map = allocate_map();
    if (map == 0) {
        int testing = allocate_pid();
        printf("%d", testing);
    } else {
        printf("error");
        return 1;
    }
    return 0;
}