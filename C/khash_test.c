#include <stdio.h>
#include <assert.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#include "khash.h"

KHASH_MAP_INIT_INT(int, unsigned char)

int main(int argc, char **argv)
{

#if 0
    time_t startTime;
    startTime = time(NULL);
    struct rusage usagebuf;
    getrusage(RUSAGE_SELF, &usagebuf);
    struct timeval startRUTime;
    startRUTime = usagebuf.ru_utime;
#endif

    int N = atoi(argv[1]);
    khash_t(int) *h;
    h = kh_init(int);
    int i,ret;
    unsigned k;
    for (i=0; i < N; ++i) {
        k = kh_put(int, h, i, &ret);
        //printf("%d ", ret);
    }

#if 0
    // Output stats.
    time_t endTime = time(NULL);
    getrusage(RUSAGE_SELF, &usagebuf);
    struct timeval endRUTime = usagebuf.ru_utime;    
    fprintf(stderr, " using %luk memory in ", usagebuf.ru_maxrss);
    fprintTimeMicroSeconds(stderr, diffTVs(&startRUTime, &endRUTime), 3);
    fprintf(stderr, " CPU seconds and ");
    fprintTimeSeconds(stderr, (endTime-startTime), 0);
    fprintf(stderr, " wall time.\n");
#endif


    /*
    printf("\n");
    for (i=0; i < 1000; ++i) {
        k = kh_put(int, h, i, &ret);
        printf("%d ", ret);
    }
    */
}
