#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "queue.h"

#define MAX_PROCS 10 // Max number of processes

/**
 * Process information.
 */
struct process {
    int pid;
};

/**
 * The process table.
 */
struct process table[MAX_PROCS];

/**
 * Initialize the process table.
 */
void init_proc_table(void)
{
    for (int i = 0; i < MAX_PROCS; i++) {
        table[i].pid = i;
    }
}

/**
 * Main.
 */
int main(void)
{
    struct queue *q = queue_new();

    init_proc_table();

    queue_enqueue(q, &table[0]);
    queue_enqueue(q, &table[1]);

    for (int i = 0; i < 15; i++){
        struct process *p = queue_dequeue(q);
        printf("PID %d: Running\n", p->pid);
        queue_enqueue(q,p);
    }

    queue_free(q);
}
