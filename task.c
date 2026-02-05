#include "task.h"

Task task_set[MAX_TASKS];
int NUM_TASKS = 0;

static int gcd(int a, int b) {
    return (b == 0) ? a : gcd(b, a % b);
}

static int lcm(int a, int b) {
    return (a * b) / gcd(a, b);
}

void GET_DATA(FILE *fp) {
    fscanf(fp, "%d", &NUM_TASKS);

    for (int i = 0; i < NUM_TASKS; i++) {
        fscanf(fp, "%d %d %d %d",
               &task_set[i].phase,
               &task_set[i].period,
               &task_set[i].wcet,
               &task_set[i].deadline);

        task_set[i].task_id = i + 1;
    }
}

void RESET_TASK_STATE(void) {
    for (int i = 0; i < NUM_TASKS; i++) {
        task_set[i].remaining_time = 0;
        task_set[i].next_release   = task_set[i].phase;
        task_set[i].abs_deadline   = 0;
        task_set[i].job_count      = 0;
        task_set[i].current_job    = 0;
        task_set[i].last_exec_time = -1;
    }
}

int COMPUTE_HYPERPERIOD(void) {
    int hp = task_set[0].period;
    for (int i = 1; i < NUM_TASKS; i++)
        hp = lcm(hp, task_set[i].period);
    return hp;
}
