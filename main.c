#include <stdio.h>
#include <stdlib.h>
#include "task.h"

void edfScheduler(int);
void rmScheduler(int);

int main(int argc, char *argv[])
{
    if (argc != 2) {
        printf("Usage: %s file\n", argv[0]);
        return 1;
    }

    FILE *fp = fopen(argv[1], "r");
    if (!fp) {
        printf("File error\n");
        return 1;
    }

    getData(fp);
    fclose(fp);

    if (numTasks <= 0) {
        printf("Tasks must be > 0\n");
        return 1;
    }

    double u = computeUtilization();
    printf("Utilization = %.3f\n", u);

    if (u > 1) {
        printf("Not schedulable (U>1)\n");
        return 0;
    }

    int hp = computeHyperperiod();

    edfScheduler(hp);
    rmScheduler(hp);

    return 0;
}