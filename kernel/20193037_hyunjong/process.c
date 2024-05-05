#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_PROCESSES 4
#define RUN_TIME 4
#define TIME_SLICE 1

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
void schedule();
void RR_process();
void IRQ_process();
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

int process_RR() {
    initialize_process_table();
    int pid1 = create_process(1024);  // 프로세스 1 생성
    int pid2 = create_process(2048);  // 프로세스 2 생성
    int pid3 = create_process(512);   // 프로세스 3 생성
    int pid4 = create_process(256);   // 프로세스 4 생성
    
    RR_process();
    terminate_process(pid1);
    terminate_process(pid2);
    terminate_process(pid3);
    terminate_process(pid4);
    display_processes();

    return 0;
}

int process_IRQ() {
    initialize_process_table();
    int pid1 = create_process(1024);  // 프로세스 1 생성
    int pid2 = create_process(2048);  // 프로세스 2 생성
    int pid3 = create_process(512);   // 프로세스 3 생성
    int pid4 = create_process(256);   // 프로세스 4 생성
    
    IRQ_process();
    terminate_process(pid1);
    terminate_process(pid2);
    terminate_process(pid3);
    terminate_process(pid4);
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

void RR_process() {
    int time = 0;
    while (time < RUN_TIME) {
        for (int i = 0; i < MAX_PROCESSES; i++) {
            if (process_table[i].state != TERMINATED) {
                process_table[i].state = RUNNING;
                printf("Process %d is running.\n", i);
                sleep(TIME_SLICE);  // 각 프로세스 실행 시간
                process_table[i].state = WAITING;
                printf("Process %d is now waiting.\n", i);
            }
        }
        time += TIME_SLICE;  // 각 프로세스의 실행 시간 추가
    }
}

void IRQ_process() {
    int elapsed_time = 0;
    while (elapsed_time < RUN_TIME) {
        for (int i = 0; i < MAX_PROCESSES; i++) {
            if (process_table[i].state != TERMINATED) {
                process_table[i].state = RUNNING;
                printf("Process %d is running.\n", i);
                sleep(1);  // 각 프로세스 실행 시간
                if (i == 1) {  // 프로세스 2 실행 중 인터럽트 발생
                    printf("Interrupt by Process 4.\n");
                    process_table[3].state = RUNNING;  // 프로세스 4 실행
                    printf("Process 4 is running.\n");
                    sleep(1);
                    process_table[3].state = WAITING;
                }
                process_table[i].state = WAITING;
                printf("Process %d is now waiting.\n", i);
            }
        }
        elapsed_time += 2;  // 각 프로세스 실행 + 인터럽트 처리 시간
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