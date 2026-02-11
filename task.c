#include "task.h"

Task taskSet[MAX_TASKS];
int numTasks = 0;

static int gcd(int a, int b) {
    return (b == 0) ? a : gcd(b, a % b);
}

static int lcm(int a, int b) {
    return (a * b) / gcd(a, b);
}

void getData(FILE *fp) {
    fscanf(fp, "%d", &numTasks);

    for (int i = 0; i < numTasks; i++) {
        fscanf(fp, "%d %d %d %d",
               &taskSet[i].phase,
               &taskSet[i].period,
               &taskSet[i].wcet,
               &taskSet[i].deadline);

        taskSet[i].taskId = i + 1;
    }
}

void resetTaskState(void) {
    for (int i = 0; i < numTasks; i++) {
        taskSet[i].remainingTime = 0;
        taskSet[i].nextRelease = taskSet[i].phase;
        taskSet[i].absDeadline = 0;
        taskSet[i].jobCount = 0;
        taskSet[i].currentJob = 0;
        taskSet[i].lastExecTime = -1;
    }
}

int computeHyperperiod(void) {
    int hp = taskSet[0].period;
    for (int i = 1; i < numTasks; i++)
        hp = lcm(hp, taskSet[i].period);
    return hp;
}

double computeUtilization(void) {
    double u = 0.0;
    for (int i = 0; i < numTasks; i++)
        u += (double)taskSet[i].wcet / taskSet[i].period;
    return u;
}
