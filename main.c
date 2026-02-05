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

    GET_DATA(fp);
    fclose(fp);

    int hyperperiod = COMPUTE_HYPERPERIOD();

    EDF_SCHEDULER(hyperperiod);
    RM_SCHEDULER(hyperperiod);

    return 0;
}
