#ifndef PQ_H
#define PQ_H

#include "task.h"

typedef struct {
    Task *data[MAX_TASKS];
    int size;
} PriorityQueue;

void pqInit(PriorityQueue *pq);
int pqEmpty(PriorityQueue *pq);

void pqPushEdf(PriorityQueue *pq, Task *t);
Task* pqPopEdf(PriorityQueue *pq);

void pqPushRm(PriorityQueue *pq, Task *t);
Task* pqPopRm(PriorityQueue *pq);

#endif
