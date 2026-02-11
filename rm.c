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

    for (int time = 0; time < hyperperiod; time++)
    {
        /* -------- Deadline Miss Check -------- */
        for (int i = 0; i < numTasks; i++)
        {
            if (taskSet[i].remainingTime > 0 &&
                time >= taskSet[i].absDeadline)
            {
                printf("\nDeadline Missed by TASK%d JOB%d at time %d\n",
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

        /* -------- Scheduling -------- */
        if (!pqEmpty(&pq))
        {
            Task *t = pqPopRm(&pq);

            printf("Time %d -> TASK%d JOB%d",
                   time,
                   t->taskId,
                   t->currentJob);

            t->remainingTime--;

            if (t->remainingTime == 0)
                printf(" (Done)\n");
            else
            {
                printf("\n");
                pqPushRm(&pq, t);
            }
        }
        else
        {
            printf("Time %d -> IDLE\n", time);
        }
    }

    printf("\nTask Set is schedulable under RM\n");
}
