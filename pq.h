#ifndef PQ_H
#define PQ_H

#include "task.h"

typedef struct {
    Task *data[MAX_TASKS];
    int size;
} PriorityQueue;

void PQ_INIT(PriorityQueue *pq);
int PQ_EMPTY(PriorityQueue *pq);

/* EDF */
void PQ_PUSH_EDF(PriorityQueue *pq, Task *t);
Task* PQ_POP_EDF(PriorityQueue *pq);

/* RM */
void PQ_PUSH_RM(PriorityQueue *pq, Task *t);
Task* PQ_POP_RM(PriorityQueue *pq);

#endif
