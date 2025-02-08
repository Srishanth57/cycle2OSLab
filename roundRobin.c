#include <stdio.h>
#include <stdlib.h>

typedef struct {
    size_t id;  // Process ID
    unsigned at; // Arrival Time
    unsigned bt; // Burst Time
    unsigned rt; // Remaining Time (for Round Robin)
} Process;

void roundRobin(Process *process, size_t n, unsigned quantum) {
    unsigned time = 0;  // Current time in the timeline
    size_t completed = 0;
    unsigned total_wt = 0, total_tt = 0;

    // Queue implementation using a simple array
    Process queue[n];
    size_t front = 0, rear = 0;
    
    // Initially, enqueue all processes
    for (size_t i = 0; i < n; ++i) {
        process[i].rt = process[i].bt;  // Set remaining burst time
        queue[rear++] = process[i];
    }

    // Execute processes in round robin fashion
    printf("\nGantt Chart: \n");
    while (completed < n) {
        // Get the first process from the queue
        Process *currentProcess = &queue[front];
        front = (front + 1) % n;
        
        if (currentProcess->rt > 0) {
            unsigned execution_time = (currentProcess->rt < quantum) ? currentProcess->rt : quantum;
            printf("P%zu -> ", currentProcess->id);
            
            currentProcess->rt -= execution_time;
            time += execution_time;
            
            // If process has finished, increment completed
            if (currentProcess->rt == 0) {
                completed++;
            }
            
            // If process still needs more time, put it back in the queue
            if (currentProcess->rt > 0) {
                queue[rear] = *currentProcess;
                rear = (rear + 1) % n;
            }
        }
    }
    printf("\n");

    // Calculate waiting time and turnaround time
    printf("\nResult Table:\n");
    printf("ID  Arrival Time  Burst Time  Waiting Time  Turnaround Time\n");

    for (size_t i = 0; i < n; ++i) {
        unsigned wt = 0, tt = 0;
        // Turnaround time is the total time minus the arrival time
        tt = process[i].bt + wt;
        total_wt += wt;
        total_tt += tt;

        printf("P%zu  %u  %u  %u  %u\n", process[i].id, process[i].at, process[i].bt, wt, tt);
    }
    printf("\nAverage Waiting Time: %.2f\n", (float)total_wt / n);
    printf("Average Turnaround Time: %.2f\n", (float)total_tt / n);
}

int main() {
    size_t n;
    unsigned quantum;

    printf("Enter number of processes: ");
    scanf("%zu", &n);

    Process process[n];
    printf("Enter process details (ID ArrivalTime BurstTime):\n");
    for (size_t i = 0; i < n; ++i) {
        scanf("%zu %u %u", &process[i].id, &process[i].at, &process[i].bt);
    }

    printf("Enter the time quantum: ");
    scanf("%u", &quantum);

    roundRobin(process, n, quantum);  // Call Round Robin scheduling function

    return 0;
}
