#include "pq.h"

/* ---------- Common ---------- */
void PQ_INIT(PriorityQueue *pq) {
    pq->size = 0;
}

int PQ_EMPTY(PriorityQueue *pq) {
    return pq->size == 0;
}

/* ---------- EDF (deadline + recency + task_id) ---------- */
void PQ_PUSH_EDF(PriorityQueue *pq, Task *t) {
    int i = pq->size++;

    while (i > 0) {
        Task *parent = pq->data[(i - 1) / 2];

        if (parent->abs_deadline < t->abs_deadline)
            break;

        if (parent->abs_deadline == t->abs_deadline) {

            if (parent->last_exec_time > t->last_exec_time)
                break;

            if (parent->last_exec_time < t->last_exec_time) {
                pq->data[i] = parent;
                i = (i - 1) / 2;
                continue;
            }

            if (parent->task_id <= t->task_id)
                break;
        }

        pq->data[i] = parent;
        i = (i - 1) / 2;
    }

    pq->data[i] = t;
}

Task* PQ_POP_EDF(PriorityQueue *pq) {
    Task *top = pq->data[0];
    Task *last = pq->data[--pq->size];

    int i = 0;
    while (2 * i + 1 < pq->size) {
        int child = 2 * i + 1;

        if (child + 1 < pq->size) {
            Task *c1 = pq->data[child];
            Task *c2 = pq->data[child + 1];

            if (c2->abs_deadline < c1->abs_deadline ||
               (c2->abs_deadline == c1->abs_deadline &&
                (c2->last_exec_time > c1->last_exec_time ||
                (c2->last_exec_time == c1->last_exec_time &&
                 c2->task_id < c1->task_id))))
                child++;
        }

        Task *c = pq->data[child];

        if (last->abs_deadline < c->abs_deadline ||
           (last->abs_deadline == c->abs_deadline &&
            (last->last_exec_time > c->last_exec_time ||
            (last->last_exec_time == c->last_exec_time &&
             last->task_id <= c->task_id))))
            break;

        pq->data[i] = c;
        i = child;
    }

    pq->data[i] = last;
    return top;
}

/* ---------- RM (period only) ---------- */
void PQ_PUSH_RM(PriorityQueue *pq, Task *t) {
    int i = pq->size++;
    while (i > 0 && pq->data[(i - 1) / 2]->period > t->period) {
        pq->data[i] = pq->data[(i - 1) / 2];
        i = (i - 1) / 2;
    }
    pq->data[i] = t;
}

Task* PQ_POP_RM(PriorityQueue *pq) {
    Task *top = pq->data[0];
    Task *last = pq->data[--pq->size];

    int i = 0;
    while (2 * i + 1 < pq->size) {
        int child = 2 * i + 1;
        if (child + 1 < pq->size &&
            pq->data[child + 1]->period <
            pq->data[child]->period)
            child++;

        if (last->period <= pq->data[child]->period)
            break;

        pq->data[i] = pq->data[child];
        i = child;
    }

    pq->data[i] = last;
    return top;
}
