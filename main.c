#include <stdio.h>
#include <stdlib.h>
#include "task.h"
#include "metrics.h"

void edfScheduler(int);
void rmScheduler(int);

int useActual = 0;  

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: %s input_file\n", argv[0]);
        return 1;
    }

    FILE *fp = fopen(argv[1], "r");
    if (!fp)
    {
        printf("File error\n");
        return 1;
    }

    getData(fp);
    fclose(fp);

    srand(42);   

    int hp = computeHyperperiod();


    // useActual = 0;

    // edfScheduler(hp);
    // printSchedule();
    // printMetrics();

    // schedSize = 0;
    // infoCount = 0;

    // rmScheduler(hp);
    // printSchedule();
    // printMetrics();




    useActual = 1;

    schedSize = 0;
    infoCount = 0;

    edfScheduler(hp);
    printSchedule();
    printMetrics();

    // schedSize = 0;
    // infoCount = 0;

    // rmScheduler(hp);
    // printSchedule();
    // printMetrics();

    free(taskSet);
    return 0;
}