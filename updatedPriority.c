

#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    size_t id;           // Process ID
    unsigned at;         // Arrival Time
    unsigned bt;         // Burst Time
    unsigned pri;        // Priority (Lower number means higher priority)
    unsigned initial_bt; // Store initial burst time
} Process;

// Sorting processes by Arrival Time

void sortByArrival(Process *process, size_t n)
{
    for (size_t i = 1; i < n; ++i)
    {
        Process key = process[i];
        int j = i - 1; // Changed to int to allow negative values
        while (j >= 0 && process[j].at > key.at)
        {
            process[j + 1] = process[j];
            j--;
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
        int j = i - 1; // Changed to int to allow negative values
        while (j >= 0 && process[j].pri > key.pri)
        {
            process[j + 1] = process[j];
            j--;
        }
        process[j + 1] = key;
    }
}

void priorityScheduling(Process *process, size_t n)
{
    // Store initial burst times
    for (size_t i = 0; i < n; ++i)
    {
        process[i].initial_bt = process[i].bt;
    }

    sortByArrival(process, n); // Sort processes by arrival time first

    unsigned time = 0; // Current time in the timeline
    printf("\nGantt Chart:\n");

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
                    currentProcess = &process[i];
                }
            }
        }

        if (currentProcess == NULL)
        {
            printf("[%u]Idle ", time); // If no process is available, show idle time
            time++;
        }
        else
        {
            printf("[%u]P%zu ", time, currentProcess->id);
            currentProcess->bt--; // Execute one unit of time for the selected process
            if (currentProcess->bt == 0)
            {
                completed++; // Process completed
            }
            time++;
        }
    }
    printf("[%u]\n", time);

    // Print result table with Waiting Time (WT) and Turnaround Time (TT)
    printf("\nResult Table:\n");
    printf("ID\tArrival Time\tBurst Time\tPriority\tWaiting Time\tTurnaround Time\n");

    unsigned total_wt = 0, total_tt = 0;

    for (size_t i = 0; i < n; ++i)
    {
        unsigned completion_time = 0;
        // Find completion time for this process
        for (size_t j = 0; j < n; ++j)
        {
            if (process[j].id == process[i].id)
            {
                for (size_t k = 0; k <= j; ++k)
                {
                    completion_time += process[k].initial_bt;
                }
                break;
            }
        }

        unsigned tt = completion_time - process[i].at;
        unsigned wt = tt - process[i].initial_bt;

        total_wt += wt;
        total_tt += tt;

        printf("%zu\t%u\t\t%u\t\t%u\t\t%u\t\t%u\n",
               process[i].id, process[i].at, process[i].initial_bt,
               process[i].pri, wt, tt);
    }

    printf("\nAverage Waiting Time: %.2f\n", (float)total_wt / n);
    printf("Average Turnaround Time: %.2f\n", (float)total_tt / n);
}

int main()
{
    size_t n;

    printf("Enter number of processes: ");
    scanf("%zu", &n);

    Process *process = malloc(n * sizeof(Process));
    if (process == NULL)
    {
        printf("Memory allocation failed\n");
        return 1;
    }

    printf("Enter process details (ID ArrivalTime BurstTime Priority):\n");
    for (size_t i = 0; i < n; ++i)
    {
        scanf("%zu %u %u %u", &process[i].id, &process[i].at, &process[i].bt, &process[i].pri);
    }

    priorityScheduling(process, n);

    free(process);
    return 0;
}