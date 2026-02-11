#include <stdio.h>
#include "rm.h"
#include "pq.h"
#include "task.h"

void rmScheduler(int hyperperiod)
{
    PriorityQueue pq;
    pqInit(&pq);
    resetTaskState();

    printf("\nRM Scheduling\n");
    printf("\nStart   End     Task    Job\n");
    printf("-----   -----   -----   -----\n");

    Task *currentTask = NULL;
    int segmentStart = 0;

    for (int time = 0; time < hyperperiod; time++)
    {
        /* -------- Deadline Miss Check -------- */
        for (int i = 0; i < numTasks; i++)
        {
            if (taskSet[i].remainingTime > 0 &&
                time >= taskSet[i].absDeadline)
            {
                if (currentTask != NULL)
                {
                    printf("%-7d %-7d T%-6d J%-6d\n",
                           segmentStart,
                           time,
                           currentTask->taskId,
                           currentTask->currentJob);
                }

                printf("\nDeadline Missed by T%d J%d at time %d\n",
                       taskSet[i].taskId,
                       taskSet[i].currentJob,
                       time);

                printf("Task Set is NOT schedulable under RM\n");
                return;
            }
        }

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

                pqPushRm(&pq, &taskSet[i]);
            }
        }

        Task *nextTask = NULL;

        if (!pqEmpty(&pq))
            nextTask = pqPopRm(&pq);

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

            if (currentTask->remainingTime > 0)
            {
                pqPushRm(&pq, currentTask);
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

    printf("\nTask Set is schedulable under RM\n");
}
