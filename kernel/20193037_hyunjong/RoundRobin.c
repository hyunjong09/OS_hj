#include <stdio.h>
#include <stdbool.h>

typedef struct {
    int processId;
    int arrivalTime;
    int burstTime;
    int remainingTime;
    int waitingTime;
    int turnaroundTime;
    int priority;
} Process;

typedef struct {
    Process* queue;
    int front;
    int rear;
    int count;
    int capacity;
    int timeQuantum;
} Queue;

void enqueue(Queue* q, Process p) {
    if (q->count < q->capacity) {
        q->rear = (q->rear + 1) % q->capacity;
        q->queue[q->rear] = p;
        q->count++;
    }
}

Process dequeue(Queue* q) {
    Process item = q->queue[q->front];
    q->front = (q->front + 1) % q->capacity;
    q->count--;
    return item;
}

bool isEmpty(Queue* q) {
    return q->count == 0;
}

int RoundRobin_priority() {
    int limit, i, total_time = 0;
    printf("Enter Total Number of Processes: ");
    scanf("%d", &limit);

    Process processes[limit];
    for(i = 0; i < limit; i++) {
        printf("\nEnter Details of Process[%d]\n", i + 1);
        printf("Arrival Time: ");
        scanf("%d", &processes[i].arrivalTime);
        printf("Burst Time: ");
        scanf("%d", &processes[i].burstTime);
        printf("Priority: ");
        scanf("%d", &processes[i].priority);
        processes[i].remainingTime = processes[i].burstTime;
        processes[i].processId = i + 1;
        processes[i].waitingTime = 0;
        processes[i].turnaroundTime = 0;
    }

    Queue highPriorityQueue = {processes, 0, -1, 0, limit, 5};
    Queue lowPriorityQueue = {processes, 0, -1, 0, limit, 10};

    for(i = 0; i < limit; i++) {
        if (processes[i].priority < 3) {
            enqueue(&highPriorityQueue, processes[i]);
        } else {
            enqueue(&lowPriorityQueue, processes[i]);
        }
    }

    printf("\nProcess Execution Details:\n");
    while(!isEmpty(&highPriorityQueue) || !isEmpty(&lowPriorityQueue)) {
        if (!isEmpty(&highPriorityQueue)) {
            Process p = dequeue(&highPriorityQueue);
            int time_run = (p.remainingTime > highPriorityQueue.timeQuantum) ? highPriorityQueue.timeQuantum : p.remainingTime;
            p.remainingTime -= time_run;
            total_time += time_run;

            printf("Process[%d] (Priority: %d) runs from %d to %d in High Priority Queue\n", p.processId, p.priority, total_time - time_run, total_time);

            if (p.remainingTime > 0) {
                enqueue(&highPriorityQueue, p);
            } else {
                p.turnaroundTime = total_time - p.arrivalTime;
                p.waitingTime = p.turnaroundTime - p.burstTime;
                printf("Process[%d] Completion - Waiting Time: %d, Turnaround Time: %d\n", p.processId, p.waitingTime, p.turnaroundTime);
            }
        } else if (!isEmpty(&lowPriorityQueue)) {
            Process p = dequeue(&lowPriorityQueue);
            int time_run = (p.remainingTime > lowPriorityQueue.timeQuantum) ? lowPriorityQueue.timeQuantum : p.remainingTime;
            p.remainingTime -= time_run;
            total_time += time_run;

            printf("Process[%d] (Priority: %d) runs from %d to %d in Low Priority Queue\n", p.processId, p.priority, total_time - time_run, total_time);

            if (p.remainingTime > 0) {
                enqueue(&lowPriorityQueue, p);
            } else {
                p.turnaroundTime = total_time - p.arrivalTime;
                p.waitingTime = p.turnaroundTime - p.burstTime;
                printf("Process[%d] Completion - Waiting Time: %d, Turnaround Time: %d\n", p.processId, p.waitingTime, p.turnaroundTime);
            }
        }
    }

    return 0;
}
