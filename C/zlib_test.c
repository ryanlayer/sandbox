#include <zlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdint.h>
#include <sys/time.h>


static struct timeval _start, _stop;

void start()
{
    gettimeofday(&_start,0);
}

void stop()
{
    gettimeofday(&_stop,0);
}

unsigned long report()
{
    return (_stop.tv_sec - _start.tv_sec) * 1000000 +  //seconds to microseconds
	    _stop.tv_usec - _start.tv_usec;
}

int main(int argc, char **argv)
{

    if (argc != 3) {
        //fprintf(stderr, "%s <num ints>\n", argv[0]);
        fprintf(stderr, "%s <num ints> <bits per int>\n", argv[0]);
        return 1;
    }

    uint32_t u_len = atoi(argv[1]);
    uint32_t bits = atoi(argv[2]);
    unsigned long u_size = u_len* sizeof(uint32_t);
    unsigned long c_size = compressBound(u_size);

    uint32_t *o = (uint32_t *) malloc(u_size);
    uint32_t *c = (uint32_t *) malloc(c_size);
    uint32_t *d = (uint32_t *) malloc(u_size);

    uint32_t *c_in = (uint32_t *) malloc(c_size);
    uint32_t *o_in = (uint32_t *) malloc(u_size);

    uint32_t i, j;
    for (i = 0; i < u_len; ++i) {
        o[i] = 0;
        for (j = 0; j < bits; ++j) 
            o[i] |= 1<<(rand()%32);
    }

    // Deflate
    int r = compress((Bytef *)c, &c_size, (Bytef *)o, u_size);
    assert(r == Z_OK);

    FILE *f = fopen(".tmp.zlib_test.c", "wb");
    fwrite(c, c_size, 1, f);
    fclose(f);

    f = fopen(".tmp.zlib_test.u", "wb");
    fwrite(o, u_size, 1, f);
    fclose(f);


    f = fopen(".tmp.zlib_test.c", "rb");
    start();
    fread(c_in, c_size, 1, f);
    stop();
    unsigned long c_read_time = report();
    start();
    r = uncompress((Bytef *)d, &u_size, (Bytef *)c_in, c_size);
    stop();
    unsigned long d_time = report();
    assert(r == Z_OK);
    fclose(f);


    f = fopen(".tmp.zlib_test.u", "rb");
    start();
    fread(o_in, u_size, 1, f);
    stop();
    unsigned long u_read_time = report();
    fclose(f);

    for (i = 0; i < u_len; ++i) {
        if (o_in[i] != d[i]) {
            printf("%u\t%u\t%u\t%u\n", i, o_in[i], o[i], d[i]);
        }
    }

    printf("u:%lu\tc:%lu\t%f\n", u_size,
                                 c_size,
                                 ((float)u_size)/((float)c_size));
    printf("u:%lu\tr:%lu\td:%lu\n", u_read_time,
                                 c_read_time,
                                 d_time);


    return 0;
}
