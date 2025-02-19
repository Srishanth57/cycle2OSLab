#include <stdio.h>
#include <stdbool.h>

typedef struct {
    size_t id;        // Process ID
    unsigned at;      // Arrival Time
    unsigned bt;      // Burst Time
    unsigned remain;  // Remaining Time
} Process;

// Sort processes by arrival time
void sortByArrival(Process *proc, size_t n) {
    for (size_t i = 1; i < n; i++) {
        Process key = proc[i];
        int j = i - 1;
        while (j >= 0 && proc[j].at > key.at) {
            proc[j + 1] = proc[j];
            j--;
        }
        proc[j + 1] = key;
    }
}

// Find process with minimum remaining time among available processes
size_t getMinRemainTimeProcess(Process *proc, size_t n, unsigned currentTime) {
    size_t minIndex = -1;
    unsigned minRemain = -1;
    
    for (size_t i = 0; i < n; i++) {
        if (proc[i].at <= currentTime && proc[i].remain > 0) {
            if (minRemain == -1 || proc[i].remain < minRemain) {
                minRemain = proc[i].remain;
                minIndex = i;
            }
        }
    }
    return minIndex;
}

void srtf(Process *proc, size_t n) {
    sortByArrival(proc, n);
    
    // Initialize remaining time
    for (size_t i = 0; i < n; i++) {
        proc[i].remain = proc[i].bt;
    }
    
    unsigned currentTime = 0;
    size_t completed = 0;
    size_t lastProc = -1;
    
    printf("\nGantt Chart:\n");
    
    while (completed < n) {
        size_t currProc = getMinRemainTimeProcess(proc, n, currentTime);
        
        if (currProc == -1) {
            if (lastProc != -1) {
                printf("%u ", currentTime);
                lastProc = -1;
            }
            printf("[ IDLE ] ");
            currentTime++;
            continue;
        }
        
        if (currProc != lastProc) {
            printf("%u [ P%zu ] ", currentTime, proc[currProc].id);
            lastProc = currProc;
        }
        
        proc[currProc].remain--;
        currentTime++;
        
        if (proc[currProc].remain == 0) {
            completed++;
        }
    }
    printf("%u\n", currentTime);
    
    // Calculate and print results
    printf("\nResults:\n");
    printf("PID\tWT\tTT\n");
    float total_wt = 0, total_tt = 0;
    
    for (size_t i = 0; i < n; i++) {
        unsigned tt = currentTime - proc[i].at;
        unsigned wt = tt - proc[i].bt;
        
        total_wt += wt;
        total_tt += tt;
        
        printf("P%zu\t%u\t%u\n", proc[i].id, wt, tt);
    }
    
    printf("\nAverage Waiting Time: %.2f\n", total_wt / n);
    printf("Average Turnaround Time: %.2f\n", total_tt / n);
}

int main() {
    size_t n;
    printf("Enter number of processes: ");
    scanf("%zu", &n);
    
    Process processes[n];
    printf("Enter process details (ID ArrivalTime BurstTime):\n");
    
    for (size_t i = 0; i < n; i++) {
        scanf("%zu %u %u", &processes[i].id, &processes[i].at, &processes[i].bt);
    }
    
    srtf(processes, n);
    return 0;
}