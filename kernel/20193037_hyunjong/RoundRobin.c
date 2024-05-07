#include <stdio.h>

// 프로세스 정보를 담는 구조체 정의
typedef struct {
    int processId;
    int arrivalTime;
    int burstTime;
    int remainingTime;
    int waitingTime;
    int turnaroundTime;
} Process;

int RoundRobin() {
    int i, limit, total = 0, time_quantum;
    float average_wait_time = 0, average_turnaround_time = 0;
    printf("Enter Total Number of Processes:\t");
    scanf("%d", &limit);
    Process processes[limit]; // 프로세스 배열 생성

    for(i = 0; i < limit; i++) {
        printf("\nEnter Details of Process[%d]\n", i + 1);
        printf("Arrival Time:\t");
        scanf("%d", &processes[i].arrivalTime);
        printf("Burst Time:\t");
        scanf("%d", &processes[i].burstTime);
        processes[i].remainingTime = processes[i].burstTime; // 남은 실행 시간 초기화
        processes[i].processId = i + 1; // 프로세스 ID 설정
        processes[i].waitingTime = 0; // 초기 대기 시간
        processes[i].turnaroundTime = 0; // 초기 반환 시간
    }

    printf("\nEnter Time Quantum:\t");
    scanf("%d", &time_quantum);
    printf("\n");

    int remaining_processes = limit; // 완료되지 않은 프로세스 수
    int start_time, end_time;

    // 모든 프로세스가 완료될 때까지 반복
    for(total = 0, i = 0; remaining_processes != 0;) {
        if(processes[i].remainingTime > 0) {
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

    // 출력 부분
    printf("\nDetailed Process Information:\n");
    for(i = 0; i < limit; i++) {
        printf("Process[%d]: Burst Time: %d, Waiting Time: %d, Turnaround Time: %d\n",
               processes[i].processId, processes[i].burstTime, processes[i].waitingTime, processes[i].turnaroundTime);
    }

    average_wait_time /= limit;
    average_turnaround_time /= limit;
    printf("\nAverage Waiting Time: %f\n", average_wait_time);
    printf("Average Turnaround Time: %f\n", average_turnaround_time);
    return 0;
}
