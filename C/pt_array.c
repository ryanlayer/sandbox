#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
 
#define NUM_THREADS     5
#define MIN(X,Y) ((X) < (Y) ? (X) : (Y))
#define MAX(X,Y) ((X) > (Y) ? (X) : (Y))
 

struct find_sum_args 
{
    int start,end;
    unsigned int *A;
};

void *find_sum(void *p)
{
    struct find_sum_args *arg = (struct find_sum_args *)p;

    unsigned int *sum = (int *) malloc (sizeof(int));

    int i;
    for (i = arg->start; i <= arg->end; ++i) 
        *sum = *sum + arg->A[i];

    return (void *)sum;
}

int main(int argc, void** argv)
{
    int num_threads = atoi(argv[1]);
    int array_size = atoi(argv[2]);

    unsigned int A[array_size];

    int i;
    for(i = 0; i < array_size; ++i)
        A[i] = i+1;

    pthread_t threads[num_threads];
    struct find_sum_args thread_args[num_threads];

    int step_size = (array_size + num_threads - 1) / num_threads;
    void *result[num_threads];
    int rc;

    for(i = 0; i < num_threads; ++i) {
        thread_args[i].start = i*step_size;
        thread_args[i].end = MIN(i*step_size + step_size - 1, array_size);
        thread_args[i].A = A;

        rc = pthread_create(&threads[i], 
                            NULL, 
                            find_sum, 
                            (void *) &thread_args[i]);
        assert(0 == rc);
    }

    /* wait for all threads to complete */
    for (i=0; i<num_threads; ++i) {
        rc = pthread_join(threads[i], &result[i]);
        assert(0 == rc);
    }

    unsigned int sum =0;
    for (i=0; i<num_threads; ++i) {
        sum += *(unsigned int *)result[i];
        printf("%d %d\n", i, *(int *)result[i]);
        free(result[i]);
    }
    printf("%d\n", sum);

    exit(EXIT_SUCCESS);
}
