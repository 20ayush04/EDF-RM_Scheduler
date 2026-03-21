#ifndef TASK_H
#define TASK_H

#define MAX_TASKS 50

typedef struct {
    int taskId;
    int phase;
    int period;
    int wcet;
    int deadline;
} Task;

typedef struct {
    int taskId;
    int jobId;

    int releaseTime;
    int absDeadline;

    int remainingTime;

    int period;          // for RM
    int lastExecTime;    // for EDF
} Job;

extern Task taskSet[MAX_TASKS];
extern int numTasks;

void getData(FILE *fp);
int computeHyperperiod(void);
double computeUtilization(void);

#endif