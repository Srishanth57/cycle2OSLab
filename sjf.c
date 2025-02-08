#include <stdio.h>
#include <stdlib.h>

typedef struct {
    size_t id;  // Process ID
    unsigned at; // Arrival Time
    unsigned bt; // Burst Time
} Process;

#define QUEUE_SIZE 32  // Define the size of the queue

typedef struct {
    Process queue[QUEUE_SIZE];
    size_t front, rear;
} Queue;

// Initialize the queue
void initQueue(Queue *q) {
    q->front = 0;
    q->rear = 0;
}

// Enqueue a process into the queue
void enqueue(Queue *q, Process p) {
    if ((q->rear + 1) % QUEUE_SIZE != q->front) {  // Check if the queue is not full
        q->queue[q->rear] = p;
        q->rear = (q->rear + 1) % QUEUE_SIZE;
    }
}

// Dequeue a process from the queue
Process dequeue(Queue *q) {
    Process p = q->queue[q->front];
    q->front = (q->front + 1) % QUEUE_SIZE;
    return p;
}

// Check if the queue is empty
int isEmpty(Queue *q) {
    return q->front == q->rear;
}

// Sorting processes by Arrival Time
void sortByArrival(Process *process, size_t n) {
    for (size_t i = 1; i < n; ++i) {
        Process key = process[i];
        size_t j = i - 1;
        while (j < n && process[j].at > key.at) {
            process[j + 1] = process[j];
            --j;
        }
        process[j + 1] = key;
    }
}

// Sorting processes by Burst Time
void sortByBurstTime(Process *process, size_t n) {
    for (size_t i = 1; i < n; ++i) {
        Process key = process[i];
        size_t j = i - 1;
        while (j < n && process[j].bt > key.bt) {
            process[j + 1] = process[j];
            --j;
        }
        process[j + 1] = key;
    }
}

void sjf(Process *process, size_t n) {
    sortByArrival(process, n);  // Sort processes by arrival time

    unsigned time = 0;  // Current time in the timeline
    Queue q;
    initQueue(&q);  // Initialize the queue

    printf("\nGantt Chart: \n");

    size_t i = 0;
    while (i < n || !isEmpty(&q)) {
        // Enqueue processes that have arrived
        while (i < n && process[i].at <= time) {
            enqueue(&q, process[i]);
            i++;
        }

        if (isEmpty(&q)) {
            printf("%u -> Idle -> ", time);  // If the queue is empty, show idle time
            time++;
        } else {
            // Sort the queue by burst time (shortest job first)
            sortByBurstTime(q.queue, (q.rear - q.front + QUEUE_SIZE) % QUEUE_SIZE);
            
            // Dequeue the next process to execute (shortest burst time)
            Process p = dequeue(&q);
            printf("P%zu -> ", p.id);
            time += p.bt;  // Update time after process completes
        }
    }
    printf("\n");

    // Print result table with Waiting Time (WT) and Turnaround Time (TT)
    printf("\nResult Table:\n");
    printf("ID  Arrival Time  Burst Time  Waiting Time  Turnaround Time\n");

    unsigned total_wt = 0, total_tt = 0;
    for (size_t i = 0; i < n; ++i) {
        unsigned wt = (i == 0) ? 0 : (process[i-1].at + process[i-1].bt - process[i].at);
        unsigned tt = wt + process[i].bt;
        total_wt += wt;
        total_tt += tt;

        printf("P%zu  %u  %u  %u  %u\n", process[i].id, process[i].at, process[i].bt, wt, tt);
    }
    printf("\nAverage Waiting Time: %.2f\n", (float)total_wt / n);
    printf("Average Turnaround Time: %.2f\n", (float)total_tt / n);
}

int main() {
    size_t n;

    printf("Enter number of processes: ");
    scanf("%zu", &n);

    Process process[n];
    printf("Enter process details (ID ArrivalTime BurstTime):\n");
    for (size_t i = 0; i < n; ++i) {
        scanf("%zu %u %u", &process[i].id, &process[i].at, &process[i].bt);
    }

    sjf(process, n);  // Call SJF scheduling function

    return 0;
}
