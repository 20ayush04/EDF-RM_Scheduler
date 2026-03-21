#ifndef PQ_H
#define PQ_H

#include "task.h"

typedef struct {
    Job data[100];
    int size;
} PriorityQueue;

void pqInit(PriorityQueue *pq);
int pqEmpty(PriorityQueue *pq);

void pqPushEdf(PriorityQueue *pq, Job j);
Job pqPopEdf(PriorityQueue *pq);

void pqPushRm(PriorityQueue *pq, Job j);
Job pqPopRm(PriorityQueue *pq);

#endif