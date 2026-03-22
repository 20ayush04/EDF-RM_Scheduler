#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "pq.h"
#include "metrics.h"

extern int useActual;   // from main.c

void rmScheduler(int hyperperiod)
{
    PQ pq;
    pqInit(&pq);

    int time = 0;
    int jobId[MAX_TASKS] = {0};

    while (time < hyperperiod)
    {
        /* -------- JOB RELEASE -------- */
        for (int i = 0; i < numTasks; i++)
        {
            if (time % taskSet[i].period == 0)
            {
                Job j;

                j.taskId = i + 1;
                j.jobId = ++jobId[i];
                j.releaseTime = time;
                j.absDeadline = time + taskSet[i].deadline;
                j.period = taskSet[i].period;

                int wcet = taskSet[i].wcet;

                /* -------- ACTUAL EXECUTION -------- */
                if (useActual)
                {
                    int minExec = (int)ceil(0.4 * wcet);
                    int maxExec = wcet;

                    j.remainingTime =
                        rand() % (maxExec - minExec + 1) + minExec;

                    printf("[RM] T%d J%d Exec=%d\n",
                           j.taskId, j.jobId, j.remainingTime);
                }
                else
                {
                    j.remainingTime = wcet;
                }

                j.totalExecution = 0;
                j.firstStartTime = -1;
                j.lastExecTime = -1;
                j.infoIndex = infoCount;

                /* -------- STORE JOB INFO -------- */
                info[infoCount++] = (JobInfo){
                    j.taskId,
                    j.jobId,
                    time,           // release
                    0,              // completion (to be filled)
                    -1,             // first start
                    j.absDeadline,
                    0               // execution
                };

                pqPushRM(&pq, j);
            }
        }

        /* -------- DEADLINE CHECK -------- */
        for (int i = 0; i < pq.size; i++)
        {
            if (pq.data[i].remainingTime > 0 &&
                time >= pq.data[i].absDeadline)
            {
                printf("\n[RM] Deadline Missed by T%d J%d at time %d\n",
                       pq.data[i].taskId,
                       pq.data[i].jobId,
                       time);

                printf("Task Set is NOT schedulable under RM\n");
                return;
            }
        }

        /* -------- IDLE CASE -------- */
        if (pqEmpty(&pq))
        {
            sched[schedSize++] =
                (Schedule){time, time + 1, -1, -1, 1};
            time++;
            continue;
        }

        /* -------- PICK HIGHEST PRIORITY -------- */
        Job j = pqPopRM(&pq);

        /* -------- FIRST START -------- */
        if (j.firstStartTime == -1)
        {
            j.firstStartTime = time;
            info[j.infoIndex].firstStartTime = time;
        }

        /* -------- EXECUTE 1 UNIT -------- */
        sched[schedSize++] =
            (Schedule){time, time + 1, j.taskId, j.jobId, 0};

        j.remainingTime--;
        j.totalExecution++;
        info[j.infoIndex].totalExecution++;

        time++;

        /* -------- COMPLETION -------- */
        if (j.remainingTime == 0)
        {
            sched[schedSize - 1].isComplete = 1;
            info[j.infoIndex].completionTime = time;
        }
        else
        {
            /* put back for future execution */
            pqPushRM(&pq, j);
        }
    }

    printf("\nRM: Task set is schedulable\n");
}   