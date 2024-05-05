#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_PROCESSES 4
#define RUN_TIME 4      // 총 실행 시간 (초)

typedef enum process_state {
    RUNNING,
    WAITING,
    TERMINATED,
    INTERRUPTED   // 추가된 INTERRUPTED 상태
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
void execute_processes();
void terminate_all_processes();
void display_processes();

int main() {
    initialize_process_table();
    create_process(1024);  // 프로세스 1 생성
    create_process(2048);  // 프로세스 2 생성
    create_process(512);   // 프로세스 3 생성
    create_process(256);   // 프로세스 4 생성, 인터럽트 관리자 역할
    
    execute_processes();
    terminate_all_processes();
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

void execute_processes() {
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

void terminate_all_processes() {
    for (int i = 0; i < MAX_PROCESSES; i++) {
        if (process_table[i].pid != -1) {
            process_table[i].state = TERMINATED;
            free(process_table[i].memory);
            process_table[i].memory = NULL;
            process_table[i].memory_size = 0;
            printf("Terminated process %d.\n", i);
        }
    }
}

void display_processes() {
    for (int i = 0; i < MAX_PROCESSES; i++) {
        if (process_table[i].pid != -1) {
            printf("Process %d, State: %d, Memory size: %d\n", i, process_table[i].state, process_table[i].memory_size);
        }
    }
}
