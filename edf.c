#include <stdio.h>
#include "edf.h"
#include "pq.h"
#include "task.h"

void edfScheduler(int hyperperiod) {

    PriorityQueue pq;
    pqInit(&pq);
    resetTaskState();

    printf("\nEDF Scheduling\n");

    for (int time = 0; time < hyperperiod; time++) {

        for (int i = 0; i < numTasks; i++) {
            if (time == taskSet[i].nextRelease) {

                taskSet[i].remainingTime = taskSet[i].wcet;
                taskSet[i].absDeadline = time + taskSet[i].deadline;
                taskSet[i].nextRelease += taskSet[i].period;

                taskSet[i].jobCount++;
                taskSet[i].currentJob = taskSet[i].jobCount;

                pqPushEdf(&pq, &taskSet[i]);
            }
        }

        if (!pqEmpty(&pq)) {

            Task *t = pqPopEdf(&pq);

            printf("Time %d -> TASK%d JOB%d",
                   time, t->taskId, t->currentJob);

            t->remainingTime--;
            t->lastExecTime = time;

            if (t->remainingTime == 0)
                printf(" (Done)\n");
            else {
                printf("\n");
                pqPushEdf(&pq, t);
            }

        } else {
            printf("Time %d -> IDLE\n", time);
        }
    }
}
