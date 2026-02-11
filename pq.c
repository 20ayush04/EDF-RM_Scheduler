#include "pq.h"

/* ---------- Common ---------- */

void pqInit(PriorityQueue *pq) {
    pq->size = 0;
}

int pqEmpty(PriorityQueue *pq) {
    return pq->size == 0;
}

/* ---------- EDF ---------- */

static int edfHigherPriority(Task *a, Task *b) {
    if (a->absDeadline != b->absDeadline)
        return a->absDeadline < b->absDeadline;

    if (a->lastExecTime != b->lastExecTime)
        return a->lastExecTime > b->lastExecTime;

    return a->taskId < b->taskId;
}

void pqPushEdf(PriorityQueue *pq, Task *t) {
    int i = pq->size++;
    while (i > 0) {
        int parent = (i - 1) / 2;
        if (edfHigherPriority(pq->data[parent], t))
            break;
        pq->data[i] = pq->data[parent];
        i = parent;
    }
    pq->data[i] = t;
}

Task* pqPopEdf(PriorityQueue *pq) {
    Task *top = pq->data[0];
    Task *last = pq->data[--pq->size];

    int i = 0;
    while (2 * i + 1 < pq->size) {
        int child = 2 * i + 1;

        if (child + 1 < pq->size &&
            edfHigherPriority(pq->data[child + 1], pq->data[child]))
            child++;

        if (edfHigherPriority(last, pq->data[child]))
            break;

        pq->data[i] = pq->data[child];
        i = child;
    }

    pq->data[i] = last;
    return top;
}

/* ---------- RM ---------- */

void pqPushRm(PriorityQueue *pq, Task *t) {
    int i = pq->size++;
    while (i > 0) {
        int parent = (i - 1) / 2;
        if (pq->data[parent]->period <= t->period)
            break;
        pq->data[i] = pq->data[parent];
        i = parent;
    }
    pq->data[i] = t;
}

Task* pqPopRm(PriorityQueue *pq) {
    Task *top = pq->data[0];
    Task *last = pq->data[--pq->size];

    int i = 0;
    while (2 * i + 1 < pq->size) {
        int child = 2 * i + 1;

        if (child + 1 < pq->size &&
            pq->data[child + 1]->period < pq->data[child]->period)
            child++;

        if (last->period <= pq->data[child]->period)
            break;

        pq->data[i] = pq->data[child];
        i = child;
    }

    pq->data[i] = last;
    return top;
}
