#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_PROCESSES 10
#define TIME_SLICE 5  // 각 프로세스에 할당된 시간 (예: 5 타임 유닛)

typedef enum process_state {
    RUNNING,
    WAITING,
    TERMINATED,
    SUSPENDED
} process_state;

typedef struct {
    int pid;
    process_state state;
    char *memory;  
    int memory_size;
    int time_remaining;  // 프로세스의 남은 실행 시간
} task_struct;

task_struct process_table[MAX_PROCESSES];

// Function prototypes
void initialize_process_table();
int create_process(int memory_size);
void round_robin_schedule();
void terminate_process(int pid);
void display_processes();

int process() {
    initialize_process_table();
    int pid1 = create_process(1024);  
    int pid2 = create_process(2048);
    
    round_robin_schedule();
    terminate_process(pid1);
    round_robin_schedule();
    display_processes();

    return 0;
}

void initialize_process_table() {
    for (int i = 0; i < MAX_PROCESSES; i++) {
        process_table[i].pid = -1;  
        process_table[i].state = TERMINATED;
        process_table[i].memory = NULL;
        process_table[i].memory_size = 0;
        process_table[i].time_remaining = 0;
    }
}

int create_process(int memory_size) {
    for (int i = 0; i < MAX_PROCESSES; i++) {
        if (process_table[i].pid == -1) {
            process_table[i].pid = i;
            process_table[i].state = WAITING;
            process_table[i].memory = (char *)malloc(memory_size);
            process_table[i].memory_size = memory_size;
            process_table[i].time_remaining = TIME_SLICE;
            printf("Created process %d with %d bytes of memory.\n", i, memory_size);
            return i;
        }
    }
    return -1;
}

void round_robin_schedule() {
    static int current_index = 0;  // 현재 프로세스 인덱스 유지
    int count = MAX_PROCESSES;
    while (count--) {
        task_struct *current_process = &process_table[current_index];
        if (current_process->pid != -1 && current_process->state != TERMINATED) {
            current_process->state = RUNNING;
            printf("Process %d is running.\n", current_index);
            // 간단한 시뮬레이션을 위해 실행 상태에서 바로 대기 상태로 전환
            current_process->state = WAITING;
        }
        current_index = (current_index + 1) % MAX_PROCESSES;  // 다음 프로세스로 이동
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
