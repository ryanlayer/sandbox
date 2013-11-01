#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
 
#define NUM_THREADS     5
 
struct pair {
    int id,a,b;
};

void *count(void *argument)
{
    struct pair *p= ((struct pair *) argument);
    int tid = p->id;

    int i;
    int sum = 0;
    for (i = p->a; i < p->b; ++i) {
        int j;
        for (j = p->a; j < p->b; ++j)
            sum += i+j;
    }

    int *sum_p = (int *) malloc(sizeof(int));
    *sum_p = sum;


    return sum_p;
}


int main(int argc, void** argv)
{
    int num_threads = atoi(argv[1]);
    int count_to = atoi(argv[2]);

    pthread_t threads[num_threads];
    struct pair thread_args[num_threads];
    int rc, i;
    void *result[num_threads];


    /* create all threads */
    for (i=0; i<num_threads; ++i) {
        thread_args[i].id = i;
        thread_args[i].a = 0;
        thread_args[i].b = count_to;
        rc = pthread_create(&threads[i], 
                            NULL, 
                            count, 
                            (void *) &thread_args[i]);
        assert(0 == rc);
    }

    /* wait for all threads to complete */
    for (i=0; i<num_threads; ++i) {
        rc = pthread_join(threads[i], &result[i]);
        assert(0 == rc);
    }

    for (i=0; i<num_threads; ++i) {
        printf("%d %d\n", i, *(int *)result[i]);
        free(result[i]);
    }

    exit(EXIT_SUCCESS);
}
