#ifndef TASK_H
#define TASK_H

#include <stdio.h>

#define MAX_TASKS 50

typedef struct {
    int taskId;

    int phase;
    int period;
    int wcet;
    int deadline;

    int remainingTime;
    int nextRelease;
    int absDeadline;

    int jobCount;
    int currentJob;

    int lastExecTime;   /* for EDF tie-breaking */
} Task;

extern Task taskSet[MAX_TASKS];
extern int numTasks;

void getData(FILE *fp);
void resetTaskState(void);
int computeHyperperiod(void);
double computeUtilization(void);

#endif
