#include <stdio.h>
#include "edf.h"
#include "pq.h"
#include "task.h"

void edfScheduler(int hyperperiod)
{
    PriorityQueue pq;
    pqInit(&pq);
    resetTaskState();

    printf("\nEDF Scheduling\n");
    printf("\nStart   End     Task    Job\n");
    printf("-----   -----   -----   -----\n");

    Task *currentTask = NULL;
    int segmentStart = 0;

    for (int time = 0; time < hyperperiod; time++)
    {
        /* -------- Job Release -------- */
        for (int i = 0; i < numTasks; i++)
        {
            if (time == taskSet[i].nextRelease)
            {
                taskSet[i].remainingTime = taskSet[i].wcet;
                taskSet[i].absDeadline = time + taskSet[i].deadline;
                taskSet[i].nextRelease += taskSet[i].period;

                taskSet[i].jobCount++;
                taskSet[i].currentJob = taskSet[i].jobCount;

                pqPushEdf(&pq, &taskSet[i]);
            }
        }

        Task *nextTask = NULL;

        if (!pqEmpty(&pq))
            nextTask = pqPopEdf(&pq);

        /* -------- Context Switch -------- */
        if (currentTask != nextTask)
        {
            if (currentTask != NULL)
            {
                printf("%-7d %-7d T%-6d J%-6d\n",
                       segmentStart,
                       time,
                       currentTask->taskId,
                       currentTask->currentJob);
            }

            segmentStart = time;
            currentTask = nextTask;
        }

        /* -------- Execute -------- */
        if (currentTask != NULL)
        {
            currentTask->remainingTime--;
            currentTask->lastExecTime = time;

            if (currentTask->remainingTime > 0)
            {
                pqPushEdf(&pq, currentTask);
            }
            else
            {
                printf("%-7d %-7d T%-6d J%-6d\n",
                       segmentStart,
                       time + 1,
                       currentTask->taskId,
                       currentTask->currentJob);

                currentTask = NULL;
            }
        }
    }

    if (currentTask != NULL)
    {
        printf("%-7d %-7d T%-6d J%-6d\n",
               segmentStart,
               hyperperiod,
               currentTask->taskId,
               currentTask->currentJob);
    }
}
