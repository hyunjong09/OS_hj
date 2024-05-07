#include <stdio.h>

typedef enum {
    READY, RUNNING, WAITING, TERMINATED
} State;

typedef struct {
    int processId;
    int arrivalTime;
    int burstTime;
    int remainingTime;
    State state;
} Process;

// 상태 이름을 문자열로 변환하는 함수
const char* getStateName(State state) {
    switch (state) {
        case READY:
            return "READY";
        case RUNNING:
            return "RUNNING";
        case WAITING:
            return "WAITING";
        case TERMINATED:
            return "TERMINATED";
        default:
            return "UNKNOWN";
    }
}

void printProcessState(Process p) {
    printf("Process[%d] %s - Remaining Time: %d\n", p.processId, getStateName(p.state), p.remainingTime);
}

int main() {
    int i, limit, total = 0, time_quantum;
    int wait_time = 0, turnaround_time = 0;
    float average_wait_time, average_turnaround_time;
    printf("Enter Total Number of Processes: ");
    scanf("%d", &limit);
    Process processes[limit];

    for(i = 0; i < limit; i++) {
        printf("\nEnter Details of Process[%d]\n", i + 1);
        printf("Arrival Time: ");
        scanf("%d", &processes[i].arrivalTime);
        printf("Burst Time: ");
        scanf("%d", &processes[i].burstTime);
        processes[i].remainingTime = processes[i].burstTime;
        processes[i].processId = i + 1;
        processes[i].state = READY;
        printProcessState(processes[i]);  // 초기 상태 출력
    }

    printf("\nEnter Time Quantum: ");
    scanf("%d", &time_quantum);
    printf("\nProcess ID\tBurst Time\tTurnaround Time\tWaiting Time\tState\n");

    int remaining_processes = limit;

    for(total = 0, i = 0; remaining_processes != 0;) {
        if(processes[i].state != TERMINATED && processes[i].state != RUNNING) {
            processes[i].state = RUNNING;
            printProcessState(processes[i]);  // 상태 변경시 출력
        }
        if(processes[i].remainingTime <= time_quantum && processes[i].remainingTime > 0) {
            total += processes[i].remainingTime;
            processes[i].remainingTime = 0;
            processes[i].state = TERMINATED;
            printProcessState(processes[i]);  // 상태 변경시 출력
            remaining_processes--;
            printf("\nProcess[%d]\t%d\t\t%d\t\t%d\t\tTERMINATED", processes[i].processId, processes[i].burstTime, total - processes[i].arrivalTime, total - processes[i].arrivalTime - processes[i].burstTime);
            wait_time += total - processes[i].arrivalTime - processes[i].burstTime;
            turnaround_time += total - processes[i].arrivalTime;
        } else if(processes[i].remainingTime > 0) {
            processes[i].remainingTime -= time_quantum;
            total += time_quantum;
            if (processes[i].remainingTime > 0) {
                processes[i].state = READY;
                printProcessState(processes[i]);  // 상태 변경시 출력
            }
        }
        if(i == limit - 1) {
            i = 0;
        } else if(processes[i + 1].arrivalTime <= total) {
            i++;
        } else {
            i = 0;
        }
    }

    average_wait_time = wait_time * 1.0 / limit;
    average_turnaround_time = turnaround_time * 1.0 / limit;
    printf("\n\nAverage Waiting Time:\t%f", average_wait_time);
    printf("\nAverage Turnaround Time:\t%f\n", average_turnaround_time);
    return 0;
}
