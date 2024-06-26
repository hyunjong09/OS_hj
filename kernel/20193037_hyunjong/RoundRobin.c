#include <stdio.h>

// 프로세스 상태를 나타내는 열거형 정의
typedef enum { NEW, READY, RUNNING, WAITING, TERMINATED } State;

// 프로세스 정보를 담는 구조체 정의
typedef struct {
    int processId;
    int arrivalTime;
    int burstTime;
    int remainingTime;
    int waitingTime;
    int turnaroundTime;
    State state;
} Process;

void sortProcessesByArrivalTime(Process processes[], int limit) {
    Process temp;
    for(int i = 0; i < limit - 1; i++) {
        for(int j = 0; j < limit - i - 1; j++) {
            if(processes[j].arrivalTime > processes[j + 1].arrivalTime) {
                temp = processes[j];
                processes[j] = processes[j + 1];
                processes[j + 1] = temp;
            }
        }
    }
}

void printState(Process process) {
    const char* stateStrings[] = { "NEW", "READY", "RUNNING", "WAITING", "TERMINATED" };
    printf("Process[%d]: %s\n", process.processId, stateStrings[process.state]);
}

int RoundRobin() {
    int i, limit, total = 0, time_quantum;
    float average_wait_time = 0, average_turnaround_time = 0;
    printf("Enter Total Number of Processes: ");
    scanf("%d", &limit);
    Process *processes = (Process *)malloc(limit * sizeof(Process));

    for(i = 0; i < limit; i++) {
        printf("\nEnter Details of Process[%d]\n", i + 1);
        printf("Arrival Time: ");
        scanf("%d", &processes[i].arrivalTime);
        printf("Burst Time: ");
        scanf("%d", &processes[i].burstTime);
        processes[i].remainingTime = processes[i].burstTime;
        processes[i].processId = i + 1;
        processes[i].waitingTime = 0;
        processes[i].turnaroundTime = 0;
        processes[i].state = NEW; // 초기 상태
        printState(processes[i]); // 초기 상태 출력
    }

    printf("\nEnter Time Quantum: ");
    scanf("%d", &time_quantum);
    printf("\n");

    sortProcessesByArrivalTime(processes, limit);

    int remaining_processes = limit; 
    int start_time, end_time;

    // 모든 프로세스가 완료될 때까지 반복
    for(total = 0, i = 0; remaining_processes != 0;) {
        if(processes[i].remainingTime > 0) {
            if (processes[i].state == NEW && processes[i].arrivalTime <= total) {
                processes[i].state = READY;
                printState(processes[i]);
            }
            if (processes[i].state == READY) {
                processes[i].state = RUNNING;
                printState(processes[i]);
            }
            start_time = total;
            if(processes[i].remainingTime <= time_quantum) {
                total += processes[i].remainingTime;
                end_time = total;
                processes[i].remainingTime = 0;
            } else {
                processes[i].remainingTime -= time_quantum;
                total += time_quantum;
                end_time = total;
            }
            printf("From %d to %d, Process[%d] was running.\n", start_time, end_time, processes[i].processId);
            if(processes[i].remainingTime == 0) {
                processes[i].turnaroundTime = total - processes[i].arrivalTime;
                processes[i].waitingTime = processes[i].turnaroundTime - processes[i].burstTime;
                average_wait_time += processes[i].waitingTime;
                average_turnaround_time += processes[i].turnaroundTime;
                processes[i].state = TERMINATED;
                printState(processes[i]);
                remaining_processes--;
            }
        }
        if(i == limit - 1) {
            i = 0;
        } else if((i + 1 < limit) && processes[i + 1].arrivalTime <= total) {
            i++;
        } else {
            i = 0;
        }
    }

    printf("\nDetailed Process Information:\n");
    for(i = 0; i < limit; i++) {
        printf("Process[%d]: Arrival Time: %d, Burst Time: %d, Waiting Time: %d, Turnaround Time: %d\n",
               processes[i].processId, processes[i].arrivalTime, processes[i].burstTime, processes[i].waitingTime, processes[i].turnaroundTime);
    }

    average_wait_time /= limit;
    average_turnaround_time /= limit;
    printf("\nAverage Waiting Time: %.3f\n", average_wait_time);
    printf("Average Turnaround Time: %.3f\n", average_turnaround_time);

    free(processes);
    return 0;
}
