#include <stdio.h>
// #include <unistd.h>
#include <fcntl.h>
// #include <sys/wait.h>
#include <sys/types.h>
// #include <sys/errno.h>
#include <unistd.h>
#define MIN_PID 300
#define MAX_PID 5000
#define MAX_QUEUE_SIZE 100
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

int process_size = 0;
int stop_time = 0;
int timer_static = 0;
int max_time = 0;
struct process
{
    char name[10];
    int priority;
    int CPU_Burst;
    int arrive_time;
    int n;
    int currently_inuse;
};
struct process *container;
int main()
{
    FILE *file = fopen("input.txt", "r");
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
    container = malloc(process_size * sizeof(struct process));
    // call the function to create the map
    allocate_map();
    int testing = allocate_pid();
    printf("%d", testing);
    // fcfs();
    roundRobin();
    return 0;
}

int allocate_map(void)
{

    printf("%d\n", process_size);
    struct process object[process_size];
    FILE *file = fopen("input.txt", "r");
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
                strcpy(object[i].name, token);
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
// timer -> working on it
void clocktimer()
{
    while (timer_static < max_time)
    {
        printf("%d", timer_static);
        fflush(stdout);
        sleep(1);
        timer_static += 1;
        printf("\r");
    }
}

void fcfs(void)
{
    putchar('\n');
    struct process readyq[process_size];
    for (int i = 0; i < process_size; i++)
    {
        strcpy(readyq[i].name, container[i].name);
        readyq[i].priority = container[i].priority;
        readyq[i].CPU_Burst = container[i].CPU_Burst;
        readyq[i].arrive_time = container[i].arrive_time;
        readyq[i].n = container[i].n;
        readyq[i].currently_inuse = container[i].currently_inuse;
    }
    // sort base on arrive time. (bubble sort)
    for (int i = 0; i < process_size - 1; i++)
    {
        for (int j = 0; j < process_size - i - 1; j++)
        {
            if (readyq[j].arrive_time > readyq[j + 1].arrive_time)
            {
                struct process temp = readyq[j];
                readyq[j] = readyq[j + 1];
                readyq[j + 1] = temp;
            }
        }
    }

    // first intiation
    max_time = readyq[0].CPU_Burst;
    printf("The process that is implemented is: %s\n", readyq[0].name);
    for (int i = 1; i < process_size; i++)
    {
        clocktimer();
        printf("The process that is implemented after is: %s\n", readyq[i].name);
        max_time = max_time + readyq[i].CPU_Burst;
    }
    // set this back to 0
    max_time = 0;
    timer_static = 0;
}

struct Queue
{
    struct process items[MAX_QUEUE_SIZE];
    int front;
    int rear;
    int size;
};

void initializeQueue(struct Queue *queue)
{
    queue->front = 0;
    queue->rear = -1;
    queue->size = 0;
}

int isQueueEmpty(struct Queue *queue)
{
    return (queue->size == 0);
}

int isQueueFull(struct Queue *queue)
{
    return (queue->size == 5);
}

void enqueue(struct Queue *queue, struct process processes)
{
    if (!isQueueFull(queue))
    {
        queue->rear = (queue->rear + 1) % MAX_QUEUE_SIZE;
        queue->items[queue->rear] = processes;
        queue->size++;
    }
    else
    {
        printf("Queue is full. Cannot enqueue.\n");
    }
}

// Dequeue a process from the queue
struct process dequeue(struct Queue *queue)
{
    struct process emptyProcess; 
    if (!isQueueEmpty(queue))
    {
        struct process processes = queue->items[queue->front];
        queue->front = (queue->front + 1) % MAX_QUEUE_SIZE;
        queue->size--;
        return processes;
    }
    else
    {
        printf("Queue is empty. Cannot dequeue.\n");
        return emptyProcess;
    }
}

void roundRobin(void)
{
    putchar('\n');
    struct process readyq[process_size];
    for (int i = 0; i < process_size; i++)
    {
        strcpy(readyq[i].name, container[i].name);
        readyq[i].priority = container[i].priority;
        readyq[i].CPU_Burst = container[i].CPU_Burst;
        readyq[i].arrive_time = container[i].arrive_time;
        readyq[i].n = container[i].n;
        readyq[i].currently_inuse = container[i].currently_inuse;
    }
    // sort base on arrive time. (bubble sort)
    for (int i = 0; i < process_size - 1; i++)
    {
        for (int j = 0; j < process_size - i - 1; j++)
        {
            if (readyq[j].arrive_time > readyq[j + 1].arrive_time)
            {
                struct process temp = readyq[j];
                readyq[j] = readyq[j + 1];
                readyq[j + 1] = temp;
            }
        }
    }

    struct Queue processqueue;
    initializeQueue(&processqueue);
    int timequa = 4;
    max_time = 0;
    int index = 0;
    enqueue(&processqueue, readyq[0]);
    int processesRemaining = process_size;

    while (processesRemaining > 0)
    {
        struct process currentProcess = dequeue(&processqueue);

        if (currentProcess.CPU_Burst <= timequa)
        {
            max_time += currentProcess.CPU_Burst;
            currentProcess.CPU_Burst = 0;
            processesRemaining--;
        }
        else
        {
            max_time += timequa;
            currentProcess.CPU_Burst -= timequa;
        }

        printf("Now the Process that is implemented is %s\n", currentProcess.name);
        clocktimer();

        while (index < process_size && readyq[index].arrive_time <= max_time)
        {
            enqueue(&processqueue, readyq[index]);
            index++;
        }

        if (currentProcess.CPU_Burst > 0)
        {
            enqueue(&processqueue, currentProcess);
        }
    }
    printf("The time is now at: %d\n", max_time);
}
