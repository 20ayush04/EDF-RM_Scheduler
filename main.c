#include <stdio.h>
#include <stdlib.h>
#include "task.h"
#include "edf.h"
#include "rm.h"

int main(int argc, char *argv[]) {

    if (argc != 2) {
        printf("Usage: %s <input_file>\n", argv[0]);
        return 1;
    }

    FILE *fp = fopen(argv[1], "r");
    if (!fp) {
        printf("File not found\n");
        return 1;
    }

    getData(fp);
    fclose(fp);

    double u = computeUtilization();

    printf("Total Utilization = %.3f\n", u);

    if (u > 1.0) {
        printf("Task Set is NOT schedulable (Utilization > 1)\n");
        return 0;
    }

    int hyperperiod = computeHyperperiod();

    edfScheduler(hyperperiod);
    rmScheduler(hyperperiod);

    return 0;
}
