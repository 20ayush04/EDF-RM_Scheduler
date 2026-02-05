#ifndef TASK_H
#define TASK_H

#include <stdio.h>

#define MAX_TASKS 50

typedef struct {
    int task_id;

    int phase;
    int period;
    int wcet;
    int deadline;

    int remaining_time;
    int next_release;
    int abs_deadline;

    int job_count;
    int current_job;

    int last_exec_time;   /* for EDF tie-breaking */
} Task;

extern Task task_set[MAX_TASKS];
extern int NUM_TASKS;

void GET_DATA(FILE *fp);
int COMPUTE_HYPERPERIOD(void);
void RESET_TASK_STATE(void);

#endif
