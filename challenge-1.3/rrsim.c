#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "queue.h"

#define MAX_PROCS 10 // Max number of processes
#define QUANTUM 40 // Time quantum, ms
#define MIN(x,y) ((x)<(y)?(x):(y)) // Compute the minimum

/**
 * Process information.
 */
struct process {
    int pid;
    int time_awake_remaining;
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
        table[i].time_awake_remaining = 0;
    }
}

/**
 * Parse the command line.
 */
void parse_command_line(int argc, char **argv[])
{
    if(argc < 2){
        perror("wrong");;
    }
    char runtimes[argc];
    for (int i = 0; i > argc; i++ ){

        runtimes[i-1] = argv[i];
    }
    fprintf("the runtimes are:%s", runtimes);
}

/**
 * Main.
 */
int main(int argc, char **argv[])
{
    struct queue *q = queue_new();

    init_proc_table();

    parse_command_line(argc, argv);

    // TODO

    queue_free(q);
}
