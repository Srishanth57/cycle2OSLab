#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define PS 10
#define QS 20
typedef int Gantt;

// Process structure
typedef struct
{
    int id; // Process ID
    int at; // arrival time
    int bt; // burst time
    int rt; // remaining time
    int ct; // completion time
    int wt; // waiting time
    int tt; // turnaround time
} Process;

typedef struct
{
    Process *data[QS];
    int front;
    int rear;
} Queue;

// Queue functions
void initQueue(Queue *q)
{
    q->front = -1;
    q->rear = -1;
}

int isEmpty(Queue *q)
{
    return (q->front == -1);
}

int isFull(Queue *q)
{
    return ((q->rear + 1) % QS == q->front);
}

void enqueue(Queue *q, Process *p)
{
    if (isFull(q))
    {
        printf("Queue overflow!\n");
        return;
    }
    else if (isEmpty(q))
    {
        q->front = 0;
        q->rear = 0;
    }
    else
    {
        q->rear = (q->rear + 1) % QS;
    }
    q->data[q->rear] = p;
}

Process *dequeue(Queue *q)
{
    if (isEmpty(q))
    {
        return NULL;
    }

    Process *p = q->data[q->front];

    // If only one element
    if (q->front == q->rear)
    {
        q->front = -1;
        q->rear = -1;
    }
    else
    {
        q->front = (q->front + 1) % QS;
    }

    return p;
}

// Update the ready queue with newly arrived processes
void updateQueue(Queue *q, Process p[], int n, int current_time)
{
    int i;
    for (i = 0; i < n; i++)
    {
        if (p[i].at == current_time && p[i].rt > 0)
        {
            enqueue(q, &p[i]);
        }
    }
}

int main()
{
    Process p[PS];
    int n, i, current_time = 0, completed = 0;
    float total_wt = 0, total_tt = 0;
    Queue rq;           // Ready Queue
    Gantt chart[1000];  // For Gantt chart, increased size for longer execution
    Process *cp = NULL; // Current Process
    int ci = 0;         // Chart Index
    int time_quantum;   // Time quantum for Round Robin

    // Initialize queue
    initQueue(&rq);

    // Input number of processes
    printf("Enter number of processes: ");
    scanf("%d", &n);

    // Input time quantum
    printf("Enter time quantum for Round Robin: ");
    scanf("%d", &time_quantum);

    // Input process details (no priority needed for Round Robin)
    for (i = 0; i < n; i++)
    {
        printf("Enter arrival time and burst time for Process %d: ", i + 1);
        p[i].id = i + 1;
        scanf("%d %d", &p[i].at, &p[i].bt);
        p[i].rt = p[i].bt;
    }

    // Round Robin scheduling
    while (completed < n)
    {
        // Add newly arrived processes to the ready queue
        updateQueue(&rq, p, n, current_time);

        // If the ready queue is empty and no process is running
        if (isEmpty(&rq) && cp == NULL)
        {
            current_time++;
            chart[ci++] = 0; // Idle time
            continue;
        }

        // Get the next process from the queue if no process is currently running
        if (cp == NULL)
        {
            cp = dequeue(&rq);
        }

        // Execute the process
        int time_slice = (cp->rt < time_quantum) ? cp->rt : time_quantum;

        // Update Gantt chart for each time unit
        for (i = 0; i < time_slice; i++)
        {
            chart[ci++] = cp->id;
            current_time++;
            cp->rt--;

            // Check for any new arrivals during this time slice
            updateQueue(&rq, p, n, current_time);
        }

        // If the process completes
        if (cp->rt == 0)
        {
            cp->ct = current_time;
            cp->tt = cp->ct - cp->at;
            cp->wt = cp->tt - cp->bt;

            total_wt += cp->wt;
            total_tt += cp->tt;

            completed++;
            cp = NULL;
        }
        else
        {
            // Process still has remaining time, put it back in the queue
            enqueue(&rq, cp);
            cp = NULL;
        }
    }

    // Print Gantt chart
    printf("\nGantt Chart:\n");

    for (i = 0; i < ci; i++)
    {
        (chart[i] == 0) ? printf("| Idle ") : printf("| P%d ", chart[i]);
    }
    printf("|\n");

    // Print process details
    printf("\nProcess\tat\tbt\tct\twt\ttt\n");
    for (i = 0; i < n; i++)
    {
        printf("P%d\t%d\t%d\t%d\t%d\t%d\n",
               p[i].id, p[i].at, p[i].bt,
               p[i].ct, p[i].wt, p[i].tt);
    }

    printf("\nAverage waiting time: %.2f", total_wt / n);
    printf("\nAverage turnaround time: %.2f\n", total_tt / n);

    return 0;
}