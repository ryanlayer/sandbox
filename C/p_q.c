#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <assert.h>

struct work_q
{
    int job;
    struct work_q *next;
};

struct work_q_args
{
    struct work_q **curr, **tail;
    int *work;
    int *waiting;
    int num_threads;
    pthread_mutex_t *work_mutex;
    pthread_cond_t *cond_mutex;
};

void *do_work(void *ptr)
{
    struct work_q_args *p = (struct work_q_args *)ptr;

    //try and grab some work

    while (1) {
        pthread_mutex_lock(p->work_mutex);

        // wait until there is work to do
        while (*(p->work) == 0) {

            // up the number waiting
            *(p->waiting) = *(p->waiting) + 1;

            // if everyone is waiting then get out
            if (*(p->waiting) == p->num_threads) {
                *(p->work) = -1;
                pthread_mutex_unlock(p->work_mutex);
                pthread_cond_signal(p->cond_mutex);
                return;
            }

            pthread_cond_wait(p->cond_mutex, p->work_mutex);

            *(p->waiting) = *(p->waiting) - 1;
        }

        // die if all the work is done
        if (*(p->work) < 0) {
            pthread_mutex_unlock(p->work_mutex);
            pthread_cond_signal(p->cond_mutex);
            return;
        }

        // Grab the work
        struct work_q *my_work = *(p->curr);
        *(p->curr) = (*(p->curr))->next;
        *(p->work) = *(p->work) - 1;

        // Release lock
        pthread_mutex_unlock(p->work_mutex);

        // do some work
        printf("%d\n", my_work->job);
        usleep(10000);

        // create the new jobs
        struct work_q *new_work_head = NULL, 
                      *new_work_tail = NULL;
        int i;
        for (i = 0; i < my_work->job; ++i) {
            struct work_q *new_work =
                    (struct work_q *)malloc(sizeof(struct work_q));
            new_work->job = my_work->job - 1;
            new_work->next = NULL;

            if (new_work_head == NULL) 
                new_work_head = new_work;
            else 
                new_work_tail->next = new_work;
                 
            new_work_tail = new_work;
        }


        //add some more work
        pthread_mutex_lock(p->work_mutex);

        if (my_work->job > 0) {

            // see if we have hit the end of the list
            if ((*(p->curr))->job == -1) {
                new_work_tail->next = *(p->curr);
                *(p->curr) = new_work_head;
                *(p->tail) = new_work_tail;
            } else {
                (*(p->tail))->next = new_work_head;
                *(p->tail) = new_work_tail;
            }

            *(p->work) = *(p->work) + my_work->job;
        } 

        pthread_mutex_unlock(p->work_mutex);
        pthread_cond_signal(p->cond_mutex);

        free(my_work);
    }
}

int main(int argc, char **argv)
{
    int num_threads = atoi(argv[1]);

    pthread_t threads[num_threads];
    struct work_q_args thread_args[num_threads];

    static pthread_cond_t cond_mutex = PTHREAD_COND_INITIALIZER;
    static pthread_mutex_t work_mutex = PTHREAD_MUTEX_INITIALIZER;

    int i,rc;

    struct work_q *head, *tail;

    struct work_q *caboose = (struct work_q *) malloc(sizeof(struct work_q));
    caboose->job = -1;
    caboose->next = NULL;


    head = (struct work_q *) malloc(sizeof(struct work_q));
    head->job = 5;
    head->next = caboose;

    tail = head;

    int work = 1;
    int waiting = 0;

    for (i = 0; i< num_threads; ++i) {
        thread_args[i].curr = &head;
        thread_args[i].tail = &tail;
        thread_args[i].work = &work;
        thread_args[i].num_threads = num_threads;
        thread_args[i].waiting = &waiting;
        thread_args[i].work_mutex = &work_mutex;
        thread_args[i].cond_mutex = &cond_mutex;

        rc = pthread_create(&threads[i],
                            NULL,
                            do_work,
                            (void *) &thread_args[i]);
        assert( 0 == rc);
    }

    for (i = 0; i < num_threads; ++i) {
        rc = pthread_join(threads[i], NULL);
        assert( 0 == rc);
    }
}
