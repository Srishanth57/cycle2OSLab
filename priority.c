#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    size_t id;    // Process ID
    unsigned at;  // Arrival Time
    unsigned bt;  // Burst Time
    unsigned pri; // Priority (Lower number means higher priority)
} Process;

// Sorting processes by Arrival Time
void sortByArrival(Process *process, size_t n)
{
    for (size_t i = 1; i < n; ++i)
    {
        Process key = process[i];
        size_t j = i - 1;
        while (j < n && process[j].at > key.at)
        {
            process[j + 1] = process[j];
            --j;
        }
        process[j + 1] = key;
    }
}

// Sorting processes by Priority (lower number means higher priority)
void sortByPriority(Process *process, size_t n)
{
    for (size_t i = 1; i < n; ++i)
    {
        Process key = process[i];
        size_t j = i - 1;
        while (j < n && process[j].pri > key.pri)
        {
            process[j + 1] = process[j];
            --j;
        }
        process[j + 1] = key;
    }
}

void priorityScheduling(Process *process, size_t n)
{
    sortByArrival(process, n); // Sort processes by arrival time first

    unsigned time = 0; // Current time in the timeline
    printf("\nGantt Chart: \n");

    // Execute processes based on priority
    size_t completed = 0;
    while (completed < n)
    {
        // Find the process with the highest priority that has arrived
        Process *currentProcess = NULL;
        for (size_t i = 0; i < n; ++i)
        {
            if (process[i].at <= time && process[i].bt > 0)
            {
                if (currentProcess == NULL || process[i].pri < currentProcess->pri)
                {
                    currentProcess = &process[i]; // Select the process with the highest priority
                }
            }
        }

        if (currentProcess == NULL)
        {
            printf("%u -> Idle -> ", time); // If no process is available, show idle time
            time++;
        }
        else
        {
            printf("P%zu -> ", currentProcess->id);
            currentProcess->bt--; // Execute one unit of time for the selected process
            if (currentProcess->bt == 0)
            {
                completed++; // Process completed
            }
            time++;
        }
    }
    printf("\n");

    // Print result table with Waiting Time (WT) and Turnaround Time (TT)
    printf("\nResult Table:\n");
    printf("ID  Arrival Time  Burst Time  Priority  Waiting Time  Turnaround Time\n");

    unsigned total_wt = 0, total_tt = 0;
    for (size_t i = 0; i < n; ++i)
    {
        unsigned wt = 0, tt = 0;
        // Calculate waiting and turnaround time for each process
        tt = process[i].bt + wt;
        total_wt += wt;
        total_tt += tt;

        printf("P%zu  %u  %u  %u  %u  %u\n", process[i].id, process[i].at, process[i].bt, process[i].pri, wt, tt);
    }
    printf("\nAverage Waiting Time: %.2f\n", (float)total_wt / n);
    printf("Average Turnaround Time: %.2f\n", (float)total_tt / n);
}

int main()
{
    size_t n;

    printf("Enter number of processes: ");
    scanf("%zu", &n);

    Process process[n];
    printf("Enter process details (ID ArrivalTime BurstTime Priority):\n");
    for (size_t i = 0; i < n; ++i)
    {
        scanf("%zu %u %u %u", &process[i].id, &process[i].at, &process[i].bt, &process[i].pri);
    }

    priorityScheduling(process, n); // Call Priority Scheduling function

    return 0;
}
