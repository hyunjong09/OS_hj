#include <stdio.h>

// 프로세스 상태를 나타내는 열거형(enum)
typedef enum {
    READY,     // 준비 상태
    RUNNING,   // 실행중 상태
    WAITING,   // 대기 상태
    TERMINATED // 완료 상태
} State;

// 프로세스 정보를 담는 구조체 정의
typedef struct {
    int processId;
    int arrivalTime;
    int burstTime;
    int remainingTime;
    State state;  // 프로세스의 현재 상태
} Process;

int main() {
    int i, limit, total = 0, counter = 0, time_quantum;
    int wait_time = 0, turnaround_time = 0;
    float average_wait_time, average_turnaround_time;
    printf("Enter Total Number of Processes:\t");
    scanf("%d", &limit);
    Process processes[limit]; // 프로세스 배열 생성

    for(i = 0; i < limit; i++) {
        printf("\nEnter Details of Process[%d]\n", i + 1);
        printf("Arrival Time:\t");
        scanf("%d", &processes[i].arrivalTime);
        printf("Burst Time:\t");
        scanf("%d", &processes[i].burstTime);
        processes[i].remainingTime = processes[i].burstTime;
        processes[i].processId = i + 1;
        processes[i].state = READY; // 초기 상태를 READY로 설정
    }

    printf("\nEnter Time Quantum:\t");
    scanf("%d", &time_quantum);
    printf("\nProcess ID\tBurst Time\tTurnaround Time\tWaiting Time\tState\n");

    int remaining_processes = limit;

    for(total = 0, i = 0; remaining_processes != 0;) {
        if(processes[i].state != TERMINATED) {
            processes[i].state = RUNNING; // 프로세스 상태를 RUNNING으로 설정
        }
        if(processes[i].remainingTime <= time_quantum && processes[i].remainingTime > 0) {
            total += processes[i].remainingTime;
            processes[i].remainingTime = 0;
            counter = 1;
            processes[i].state = TERMINATED; // 프로세스 완료
        } else if(processes[i].remainingTime > 0) {
            processes[i].remainingTime -= time_quantum;
            total += time_quantum;
        }
        if(processes[i].state == TERMINATED && counter == 1) {
            remaining_processes--;
            printf("\nProcess[%d]\t%d\t\t%d\t\t%d\t\t%s", processes[i].processId, processes[i].burstTime, total - processes[i].arrivalTime, total - processes[i].arrivalTime - processes[i].burstTime, "TERMINATED");
            wait_time += total - processes[i].arrivalTime - processes[i].burstTime;
            turnaround_time += total - processes[i].arrivalTime;
            counter = 0;
        } else {
            processes[i].state = READY; // 프로세스 상태를 READY로 재설정
        }
        if(i == limit - 1) {
            i = 0;
        } else if(arrival_time[i + 1] <= total) {
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
