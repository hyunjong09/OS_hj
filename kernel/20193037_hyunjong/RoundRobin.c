#include<stdio.h>

// 프로세스 정보를 담는 구조체 정의
typedef struct {
    int processId;
    int arrivalTime;
    int burstTime;
    int remainingTime;
} Process;

int RoundRobin() {
    int i, limit, total = 0, counter = 0, time_quantum;
    int wait_time = 0, turnaround_time = 0;
    float average_wait_time, average_turnaround_time;
    printf("Enter Total Number of Processes:\t");
    scanf("%d", &limit);
    Process processes[limit]; // 프로세스 배열 생성

    for(i = 0; i < limit; i++) {
        printf("nEnter Details of Process[%d]\n", i + 1);
        printf("Arrival Time:\t");
        scanf("%d", &processes[i].arrivalTime);
        printf("Burst Time:\t");
        scanf("%d", &processes[i].burstTime);
        processes[i].remainingTime = processes[i].burstTime; // 남은 실행 시간 초기화
        processes[i].processId = i + 1; // 프로세스 ID 설정
    }

    printf("nEnter Time Quantum:\t");
    scanf("%d", &time_quantum);
    printf("\nProcess ID\tBurst Time\tTurnaround Time\tWaiting Time\n");

    int remaining_processes = limit; // 완료되지 않은 프로세스 수

    // 모든 프로세스가 완료될 때까지 반복
    for(total = 0, i = 0; remaining_processes != 0;) {
        if(processes[i].remainingTime <= time_quantum && processes[i].remainingTime > 0) {
            total += processes[i].remainingTime;
            processes[i].remainingTime = 0;
            counter = 1;
        } else if(processes[i].remainingTime > 0) {
            processes[i].remainingTime -= time_quantum;
            total += time_quantum;
        }
        if(processes[i].remainingTime == 0 && counter == 1) {
            remaining_processes--;
            printf("\nProcess[%d]\t\t%d\t\t%d\t\t%d", processes[i].processId, processes[i].burstTime, total - processes[i].arrivalTime, total - processes[i].arrivalTime - processes[i].burstTime);
            wait_time += total - processes[i].arrivalTime - processes[i].burstTime;
            turnaround_time += total - processes[i].arrivalTime;
            counter = 0;
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
