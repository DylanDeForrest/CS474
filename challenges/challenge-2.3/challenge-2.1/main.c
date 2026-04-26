#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdatomic.h>

#define MAX_THREADS 20

atomic_flag flag = ATOMIC_FLAG_INIT;
int crit_count;

/**
 * Lock spinlock.
 */
void lock(void)
{
    while(atomic_flag_test_and_set(&flag)) {
        // spin
    }
}

/**
 * Unlock spinlock.
 */
void unlock(void)
{
    atomic_flag_clear(&flag);
}

/**
 * Run function for threads to simulate work.
 */
void *run(void *args)
{
    int id = *(int*)args;

    lock();
    printf("Thread %d: In critical section\n", id);
    crit_count++;
    if (crit_count > 1) {
        printf("Yikes! %d threads in the critical section!\n", crit_count);
    }
    usleep(50000); // 50 ms
    crit_count--;
    unlock();

    return NULL;
}

/**
 * Main. Create the threads and join them.
 */
int main(int argc, char *argv[])
{
    pthread_t t[MAX_THREADS];
    int id[MAX_THREADS];

    if (argc != 2) {
        fprintf(stderr, "usage: %s threadcount\n", argv[0]);
        return 1;
    }

    int thread_count = atoi(argv[1]);

    if (thread_count < 1 || thread_count > MAX_THREADS) {
        fprintf(stderr, "%s: thread count must be betweeen 1 and %d\n", argv[0], MAX_THREADS);
        return 2;
    }

    for (int i = 0; i < thread_count; i++) {
        id[i] = i;
        pthread_create(t + i, NULL, run, id + i);
    }

    for (int i = 0; i < thread_count; i++) {
        pthread_join(t[i], NULL);
    }
}