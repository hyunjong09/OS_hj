#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_PROCESSES 10

typedef enum process_state {
    RUNNING,
    WAITING,
    TERMINATED
} process_state;

typedef struct {
    int pid;
    process_state state;
    char *memory;  
    int memory_size;
} task_struct;

task_struct process_table[MAX_PROCESSES];

// Function prototypes
void initialize_process_table();
int create_process(int memory_size);
void schedule();
void terminate_process(int pid);
void display_processes();

int process() {
    initialize_process_table();
    int pid1 = create_process(1024);  
    int pid2 = create_process(2048);
    
    schedule();
    terminate_process(pid1);
    schedule();
    display_processes();

    return 0;
}

void initialize_process_table() {
    for (int i = 0; i < MAX_PROCESSES; i++) {
        process_table[i].pid = -1;  
        process_table[i].state = TERMINATED;
        process_table[i].memory = NULL;
        process_table[i].memory_size = 0;
    }
}

int create_process(int memory_size) {
    for (int i = 0; i < MAX_PROCESSES; i++) {
        if (process_table[i].pid == -1) {
            process_table[i].pid = i;
            process_table[i].state = WAITING;
            process_table[i].memory = (char *)malloc(memory_size);
            process_table[i].memory_size = memory_size;
            printf("Created process %d with %d bytes of memory.\n", i, memory_size);
            return i;
        }
    }
    return -1;
}

void schedule() {
    for (int i = 0; i < MAX_PROCESSES; i++) {
        if (process_table[i].pid != -1 && process_table[i].state != TERMINATED) {
            process_table[i].state = RUNNING;
            printf("Process %d is running.\n", i);
        }
    }
}

void terminate_process(int pid) {
    if (pid >= 0 && pid < MAX_PROCESSES && process_table[pid].pid != -1) {
        process_table[pid].state = TERMINATED;
        free(process_table[pid].memory);
        process_table[pid].memory = NULL;
        process_table[pid].memory_size = 0;
        printf("Terminated process %d.\n", pid);
    }
}

void display_processes() {
    for (int i = 0; i < MAX_PROCESSES; i++) {
        if (process_table[i].pid != -1) {
            printf("Process %d, State: %d, Memory size: %d\n", i, process_table[i].state, process_table[i].memory_size);
        }
    }
}