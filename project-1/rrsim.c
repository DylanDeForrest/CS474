#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "queue.h"

#define MAX_PROG_LEN 12  // Max terms in a "program"
#define MAX_PROCS 20  // Max number of processes
#define QUANTUM   40  // Time quantum, ms
#define MIN(x,y) ((x)<(y)?(x):(y)) // Compute the minimum

struct process {
    int pid;
    int state;               
    int time_awake_remaining;
    int time_sleep_remaining;
    int pc;                 
    int program[MAX_PROG_LEN];  
};


struct process table[MAX_PROCS];
int num_procs = 0;


void init_proc_table(void)
{
    for (int i = 0; i < MAX_PROCS; i++) {
        table[i].pid                = i;
        table[i].state              = 0;
        table[i].time_awake_remaining = 0;
        table[i].time_sleep_remaining = 0;
        table[i].pc                 = 0;
    }
}


void parse_command_line(int argc, char **argv)
{
    num_procs = argc - 1;

    for (int i = 0; i < num_procs; i++) {
        char buf[256];
        strcpy(buf, argv[i + 1]);

        int idx = 0;
        char *tok = strtok(buf, ",");
        while (tok != NULL) {
            table[i].program[idx++] = atoi(tok);
            tok = strtok(NULL, ",");
        }
        table[i].program[idx] = 0; 

        table[i].time_awake_remaining = table[i].program[0];
    }
}


int main(int argc, char **argv)
{
    int clock = 0;

    struct queue *q = queue_new();

    init_proc_table();
    parse_command_line(argc, argv);

    for (int i = 0; i < num_procs; i++)
        queue_enqueue(q, &table[i]);

    int active = num_procs;

    while (active > 0) {

        if (queue_is_empty(q)) {
            int min = -1;
            for (int i = 0; i < num_procs; i++) {
                if (table[i].state == 1) {
                    if (min < 0 || table[i].time_sleep_remaining < min)
                        min = table[i].time_sleep_remaining;
                }
            }
            clock += min;
            for (int i = 0; i < num_procs; i++)
                if (table[i].state == 1)
                    table[i].time_sleep_remaining -= min;
        }

        printf("=== Clock %d ms ===\n", clock);

        for (int i = 0; i < num_procs; i++) {
            if (table[i].state == 1 && table[i].time_sleep_remaining <= 0) {
                table[i].pc++;
                int next = table[i].program[table[i].pc];
                if (next == 0) {
                    table[i].state = 2;
                    active--;
                    printf("PID %d: Exiting\n", table[i].pid);
                } else {
                    table[i].state                = 0;
                    table[i].time_awake_remaining = next;
                    printf("PID %d: Waking up for %d ms\n", table[i].pid, next);
                    queue_enqueue(q, &table[i]);
                }
            }
        }

        if (queue_is_empty(q))
            continue;

        struct process *p = queue_dequeue(q);
        printf("PID %d: Running\n", p->pid);

        int run_time = MIN(QUANTUM, p->time_awake_remaining);

        clock += run_time;
        p->time_awake_remaining -= run_time;
        for (int i = 0; i < num_procs; i++)
            if (table[i].state == 1)
                table[i].time_sleep_remaining -= run_time;

        if (p->time_awake_remaining > 0) {
            queue_enqueue(q, p);
        } else {
            p->pc++;
            int next = p->program[p->pc];
            if (next == 0) {
                p->state = 2;
                active--;
                printf("PID %d: Exiting\n", p->pid);
            } else {
                p->state                = 1;
                p->time_sleep_remaining = next;
                printf("PID %d: Sleeping for %d ms\n", p->pid, next);
            }
        }

        printf("PID %d: Ran for %d ms\n", p->pid, run_time);
    }

    queue_free(q);
}