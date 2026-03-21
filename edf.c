#include <stdio.h>
#include "pq.h"
#include "task.h"

void edfScheduler(int hyperperiod)
{
    PriorityQueue pq;
    pqInit(&pq);

    int time = 0;
    int jobId[MAX_TASKS] = {0};

    printf("\nEDF Scheduling\n");
    printf("Start   End     Task    Job\n");

    while (time < hyperperiod)
    {
        /* release jobs */
        for (int i = 0; i < numTasks; i++) {
            if (time % taskSet[i].period == 0) {
                Job j;
                j.taskId = taskSet[i].taskId;
                j.jobId = ++jobId[i];
                j.releaseTime = time;
                j.absDeadline = time + taskSet[i].deadline;
                j.remainingTime = taskSet[i].wcet;
                j.period = taskSet[i].period;
                j.lastExecTime = -1;

                pqPushEdf(&pq, j);
            }
        }

        if (pqEmpty(&pq)) {
            time++;
            continue;
        }

        Job j = pqPopEdf(&pq);

        int exec = j.remainingTime;
        int nextRelease = hyperperiod;

        for (int i = 0; i < numTasks; i++) {
            int nr = ((time / taskSet[i].period) + 1) * taskSet[i].period;
            if (nr < nextRelease) nextRelease = nr;
        }

        if (time + exec > nextRelease)
            exec = nextRelease - time;

        printf("%-7d %-7d T%-6d J%-6d\n",
               time, time + exec, j.taskId, j.jobId);

        j.remainingTime -= exec;
        j.lastExecTime = time;

        time += exec;

        if (j.remainingTime > 0)
            pqPushEdf(&pq, j);
    }
}