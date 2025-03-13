

#include <stdio.h>

int main()
{

    int n, r, i, j;
    printf("Enter the no of process and resources");

    scanf("%d%d", &n, &r);

    int max[n][r], alloc[n][r], need[n][r], avail[r], work[r], finish[n];
    printf("Enter the max matrix\n");
    for (int i = 0; i < n; i++)
    {
        printf("Enter the resources for P%d", i);
        for (int j = 0; j < r; j++)
        {
            scanf("%d", &max[i][j]);
        }
    }
    printf("Enter the allocation matrix\n");
    for (int i = 0; i < n; i++)
    {
        printf("Enter the resources for P%d", i);
        for (int j = 0; j < r; j++)
        {
            scanf("%d", &alloc[i][j]);
        }
    }
    printf("Enter the available resources");
    for (int i = 0; i < r; i++)
    {
        scanf("%d", &avail[i]);
        work[i] = avail[i];
    }

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < r; j++)
        {
            need[i][j] = max[i][j] - alloc[i][j];
        }
        finish[i] = 0;
    }

    int safeState[n];
    // safe check algo
    int found = 0;
    int count = 0;

    do
    {

        found = 0;

        for (int i = 0; i < n; i++)
        {
            if (!finish[i])
            {
                int safe = 1;
                for (int j = 0; j < r; j++)
                {
                    if (need[i][j] > work[j])
                    {
                        safe = 0;
                        break;
                    }
                }
                if (safe)
                {
                    for (int j = 0; j < r; j++)
                    {
                        work[j] += alloc[i][j];
                    }
                    found = 1;
                    finish[i] = 1;
                    safeState[count++] = i;
                }
            }
        }
    } while (found && count < n);

    if (count == n)
    {
        printf("Safe State");
        for (int i = 0; i < n; i++)
        {
            printf("%d", safeState[i]);
        }
    }
    else
    {
        printf("Not in safe state");
    }
    return 0;
}