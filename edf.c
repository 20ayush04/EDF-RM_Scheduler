#include <stdio.h>
#include "edf.h"
#include "pq.h"
#include "task.h"

void EDF_SCHEDULER(int hyperperiod) {
    PriorityQueue pq;
    PQ_INIT(&pq);
    RESET_TASK_STATE();

    printf("\nEDF Scheduling\n");

    for (int time = 0; time < hyperperiod; time++) {

        for (int i = 0; i < NUM_TASKS; i++) {
            if (time == task_set[i].next_release) {
                task_set[i].remaining_time = task_set[i].wcet;
                task_set[i].abs_deadline   = time + task_set[i].deadline;
                task_set[i].next_release  += task_set[i].period;

                task_set[i].job_count++;
                task_set[i].current_job = task_set[i].job_count;

                PQ_PUSH_EDF(&pq, &task_set[i]);
            }
        }

        if (!PQ_EMPTY(&pq)) {
            Task *t = PQ_POP_EDF(&pq);

            printf("Time %d -> TASK%d JOB%d",
                   time, t->task_id, t->current_job);

            t->remaining_time--;
            t->last_exec_time = time;

            if (t->remaining_time == 0) {
                printf(" (Done)\n");
            } else {
                printf("\n");
                PQ_PUSH_EDF(&pq, t);
            }
        } else {
            printf("Time %d -> IDLE\n", time);
        }
    }
}
