#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_PROCESSES 4
#define SLEEP_INTERVAL 2 // 스케줄링 간 딜레이 시간 (초)
#define RUN_TIME 30      // 총 실행 시간 (초)

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

void initialize_process_table();
int create_process(int memory_size);
void round_robin_schedule();
void terminate_process(int pid);
void display_processes();

int main() {
    initialize_process_table();
    create_process(1024);  // 프로세스 1 생성
    create_process(2048);  // 프로세스 2 생성
    create_process(512);   // 프로세스 3 생성
    create_process(256);   // 프로세스 4 생성
    
    int elapsed_time = 0;
    while (elapsed_time < RUN_TIME) {
        round_robin_schedule();
        sleep(SLEEP_INTERVAL);
        elapsed_time += SLEEP_INTERVAL;
    }

    // 모든 프로세스 종료
    for (int i = 0; i < MAX_PROCESSES; i++) {
        terminate_process(i);
    }

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
    static int pid = 0;  // PID를 순차적으로 할당
    if (pid < MAX_PROCESSES) {
        process_table[pid].pid = pid;
        process_table[pid].state = WAITING;
        process_table[pid].memory = (char *)malloc(memory_size);
        process_table[pid].memory_size = memory_size;
        printf("Created process %d with %d bytes of memory.\n", pid, memory_size);
        pid++;
        return pid - 1;
    }
    return -1;
}

void round_robin_schedule() {
    for (int i = 0; i < MAX_PROCESSES; i++) {
        if (process_table[i].pid != -1 && process_table[i].state != TERMINATED) {
            process_table[i].state = RUNNING;
            printf("Process %d is running.\n", i);
            sleep(SLEEP_INTERVAL);
            process_table[i].state = WAITING;
            printf("Process %d is now waiting.\n", i);
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
