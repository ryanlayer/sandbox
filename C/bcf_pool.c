#include <stdio.h>
#include <htslib/hts.h>
#include <htslib/vcf.h>
#include <htslib/kstring.h>
#include <strings.h>
#include <pthread.h>
#include <sys/types.h>
#include <assert.h>
#include "timer.h"
#include "pq.h"

uint32_t bcf_push(uint32_t var_num,
                  bcf_hdr_t *hdr,
                  bcf1_t *line,
                  uint32_t *packed_ints,
                  uint32_t num_ints,
                  uint32_t *md_index,
                  uint32_t *md_i,
                  pri_queue *q,
                  FILE *gt_of,
                  FILE *md_of);

struct bcf_push_args
{
    uint32_t id;
    //bcf1_t *line;
    bcf_hdr_t *hdr;
    uint32_t *packed_ints, *md_index;
    uint32_t num_ints, num_mds, num_threads;
    pri_queue *q;
    FILE *gt_of, *md_of;
    pthread_cond_t *new_work_mutex;
    pthread_mutex_t *q_mutex;
    struct line_q **head, **tail;
    int *done, *work;
};

struct line_q
{
    bcf1_t *line;
    struct line_q *next;
};


void *do_work(void *ptr)
{
    struct bcf_push_args *p = (struct bcf_push_args *)ptr;

    uint32_t var_id = 0, sum = 0, md_i = 0;


    while (1) {
        //try and grab some work
        pthread_mutex_lock(p->q_mutex);

        // wait until there is work to do
        while (*(p->work) == 0) {
            if ((*(p->work) == 0) && (*(p->done) == -1)) {
                pthread_mutex_unlock(p->q_mutex);
                pthread_cond_signal(p->new_work_mutex);
                return NULL;
            }

            pthread_cond_wait(p->new_work_mutex, p->q_mutex);
        }

        // die if all the work is done
        if ((*(p->work) == 0) && (*(p->done) == -1)) {
            pthread_mutex_unlock(p->q_mutex);
            pthread_cond_signal(p->new_work_mutex);
            return NULL;
        }

        //fprintf(stderr, "GET WORK %d %p\n", *(p->work), *(p->head));
        // Grab the work
        struct line_q *my_work = *(p->head);
        bcf1_t *line = my_work->line;
        *(p->head) = (*(p->head))->next;
        *(p->work) = *(p->work) - 1;

        // Release lock
        //fprintf(stderr, "GOT WORK\n");
        pthread_mutex_unlock(p->q_mutex);

        sum += bcf_push(var_id,
                        p->hdr,
                        line,
                        p->packed_ints,
                        p->num_ints,
                        p->md_index,
                        &md_i,
                        p->q,
                        p->gt_of,
                        p->md_of);
        var_id += 1;
        free(my_work);

    }

    return NULL;
}

//{{{uint32_t bcf_push(uint32_t var_num,
uint32_t bcf_push(uint32_t var_num,
                  bcf_hdr_t *hdr,
                  bcf1_t *line,
                  uint32_t *packed_ints,
                  uint32_t num_ints,
                  uint32_t *md_index,
                  uint32_t *md_i,
                  pri_queue *q,
                  FILE *gt_of,
                  FILE *md_of)
{
    bcf_unpack(line, BCF_UN_ALL);

    size_t len = strlen(bcf_hdr_id2name(hdr, line->rid)) +
                 10 + // max length of pos
                 strlen(line->d.id) +
                 strlen(line->d.allele[0]) +
                 strlen(line->d.allele[1]) +
                 4; //tabs
    char *md = (char *) malloc(len * sizeof(char));

    sprintf(md, "%s\t%d\t%s\t%s\t%s",
                 bcf_hdr_id2name(hdr, line->rid),
                 line->pos + 1,
                 line->d.id,
                 line->d.allele[0],
                 line->d.allele[1]); 

    *md_i += strlen(md);
    md_index[var_num] = *md_i;
    fprintf(md_of, "%s", md);

    int32_t *gt_p = NULL, ntmp = 0;

    uint32_t num_gts_per_sample = bcf_get_genotypes(hdr,
                                                    line,
                                                    &gt_p,
                                                    &ntmp);


    uint32_t num_samples = bcf_hdr_nsamples(hdr);
    num_gts_per_sample /= num_samples;

    int32_t *gt_i = gt_p;

    uint32_t j, k, int_i = 0, two_bit_i = 0, sum = 0;
    memset(packed_ints, 0, num_ints * sizeof(uint32_t));

    for (j = 0; j < num_samples; ++j) {
        uint32_t gt = 0;
        for (k = 0; k < num_gts_per_sample; ++k) {
            gt += bcf_gt_allele(gt_i[k]);
        }

        packed_ints[int_i] += gt << (30 - 2*two_bit_i);
        two_bit_i += 1;
        if (two_bit_i == 16) {
            two_bit_i = 0;
            int_i += 1;
        }

        sum += gt;
        gt_i += num_gts_per_sample;
    }

    priority p;
    p.sum = sum;
    p.len = 0;
    int *d = (int *) malloc (sizeof(int));
    d[0] = var_num;
    priq_push(*q, d, p);

    fwrite(packed_ints, sizeof(uint32_t), num_ints, gt_of);

    bcf_destroy(line);
    free(md);
    free(gt_p);

    return sum;
}
//}}}

int main(int argc, char **argv)
{

    if (argc < 4) {
        fprintf(stderr,"%s <bcf file> <num vars> <num threads>\n", argv[0]);
        return 1;
    }

    char *fname = argv[1];
    uint32_t num_vars = atoi(argv[2]);
    uint32_t num_threads = atoi(argv[3]);

    htsFile *fp    = hts_open(fname,"rb");
    bcf_hdr_t *hdr = bcf_hdr_read(fp);
    bcf1_t *line    = bcf_init1();
    //int32_t *gt_p = NULL;

    uint32_t num_records = bcf_hdr_nsamples(hdr);
    printf("num_records:%u\n", num_records);

    int32_t i, t_sum = 0;
    
    uint32_t num_ints = 1 + ((num_records - 1) / 16);

    // allocte blocks of space for the different threads
    uint32_t *packed_ints_block = 
            (uint32_t *) malloc(num_threads*num_ints*sizeof(uint32_t));
    uint32_t **Packed_ints = 
            (uint32_t **) malloc(num_threads*sizeof(uint32_t *));
    for (i = 0; i < num_threads; ++i)
        Packed_ints[i] = packed_ints_block + i*num_ints;

    pri_queue *Q = (pri_queue *) malloc(num_threads*sizeof(pri_queue));
    for (i = 0; i < num_threads; ++i)
        Q[i] = priq_new(0);

    FILE **GT_of = (FILE **) malloc(num_threads*sizeof(FILE));
    for (i = 0; i < num_threads; ++i) {
        char name[13];
        sprintf(name, "gt.%d", i);
        GT_of[i] = fopen(name, "wb");
    }

    FILE **MD_of = (FILE **) malloc(num_threads*sizeof(FILE));
    for (i = 0; i < num_threads; ++i) {
        char name[13];
        sprintf(name, "md.%d", i);
        MD_of[i] = fopen(name, "w");
    }

    uint32_t *MD_index_block =
        (uint32_t *) malloc(num_threads*num_vars*sizeof(uint32_t));
    uint32_t **MD_index = 
        (uint32_t **) malloc(num_threads*sizeof(uint32_t *));
    for (i = 0; i < num_threads; ++i) 
        MD_index[i] = MD_index_block + i*num_records;


    struct line_q *head, *tail;
    head = NULL;
    tail = head;

    static pthread_cond_t new_work_mutex = PTHREAD_COND_INITIALIZER;
    static pthread_mutex_t q_mutex = PTHREAD_MUTEX_INITIALIZER;
    
    pthread_t threads[num_threads];
    struct bcf_push_args thread_args[num_threads];
    //int waiting = 0;
    int work = 0;
    int done = 0;
    for (i = 0; i < num_threads; ++i) {
        thread_args[i].hdr = hdr;
        thread_args[i].id = i;
        thread_args[i].packed_ints = Packed_ints[i];
        thread_args[i].md_index = MD_index[i];
        thread_args[i].num_ints = num_ints;
        thread_args[i].num_mds = 0;
        thread_args[i].q = &(Q[i]);
        thread_args[i].gt_of = GT_of[i];
        thread_args[i].md_of = MD_of[i];
        thread_args[i].new_work_mutex = &new_work_mutex;
        thread_args[i].q_mutex = &q_mutex;
        thread_args[i].head = &head;
        thread_args[i].tail = &tail;
        thread_args[i].work = &work;
        thread_args[i].done = &done;
        thread_args[i].num_threads = num_threads;
        int rc = pthread_create(&threads[i],
                                NULL,
                                do_work,
                                (void *) &thread_args[i]);

        assert( 0 == rc);
    };

    for (i = 0; i < num_vars; ++i) {
        //fprintf(stderr, "i:%u\tnum_vars:%u\twork:%u\n", i, num_vars, work);
        int r = bcf_read(fp, hdr, line);
        bcf1_t *t_line = bcf_dup(line);

        struct line_q *new_q = (struct line_q *) malloc(sizeof(struct line_q));
        new_q->line = t_line;
        new_q->next = NULL;

        pthread_mutex_lock(&q_mutex);
        //fprintf(stderr, "START ADD\n");
        if (head == NULL)
            head = new_q;
        else
            tail->next = new_q;

        tail = new_q;
        //fprintf(stderr, "END ADD %d\n", work);
        work += 1;
        pthread_mutex_unlock(&q_mutex);
        pthread_cond_signal(&new_work_mutex);
    }

    done = -1;

    for (i = 0; i < num_threads; ++i) {
        int rc = pthread_join(threads[i], NULL);
        assert( 0 == rc);
    }

    free(packed_ints_block);
    free(Packed_ints);
    for (i = 0; i < num_threads; ++i) {
        priq_free(Q[i]);
        fclose(GT_of[i]);
        fclose(MD_of[i]);
    }
    free(Q);
    free(GT_of);
    free(MD_of);
    free(MD_index_block);
    free(MD_index);
    hts_close(fp);
    bcf_hdr_destroy(hdr);
    bcf_destroy(line);

#if 0
    unsigned long t_bcf_read = 0, 
                  t_bcf_dup = 0,
                  t_bcf_unpack = 0,
                  t_bcf_get_genotypes = 0,
                  t_bcf_hdr_nsamples = 0,
                  t_q = 0,
                  t_write = 0,
                  t_get_md = 0,
                  t_md_write = 0,
                  t_pack = 0;

    for (i = 0; i < num_records; ++i) {
        sum = 0;
        int_i = 0;

        // Read
        start();
        int r = bcf_read(fp, hdr, line);
        stop();
        t_bcf_read += report();
        
        // Copy
        start();
        bcf1_t *t_line = bcf_dup(line);
        stop();
        t_bcf_dup += report();

        t_sum += bcf_push(i,
                          hdr,
                          t_line,
                          packed_ints,
                          num_ints,
                          md_index,
                          &md_i,
                          &q,
                          gt_of,
                          md_of);
#if 0
//{{{
        // Unpack
        start();
        bcf_unpack(t_line, BCF_UN_ALL);
        stop();
        t_bcf_unpack += report();

        // Get metadata
        start();
        size_t len = strlen(bcf_hdr_id2name(hdr, t_line->rid)) +
                     10 + // max length of pos
                     strlen(t_line->d.id) +
                     strlen(t_line->d.allele[0]) +
                     strlen(t_line->d.allele[1]) +
                     4; //tabs
        char *md = (char *) malloc(len * sizeof(char));

        sprintf(md, "%s\t%d\t%s\t%s\t%s",
                     bcf_hdr_id2name(hdr, t_line->rid),
                     t_line->pos + 1,
                     t_line->d.id,
                     t_line->d.allele[0],
                     t_line->d.allele[1]); 
        stop();
        t_get_md += report();

        // Write metadata
        start();
        md_i += strlen(md);
        md_index[i] = md_i;
        fprintf(md_of, "%s", md);
        stop();
        t_md_write += report();

        // Get gentotypes
        start();
        uint32_t num_gts_per_sample = bcf_get_genotypes(hdr,
                                                        t_line,
                                                        &gt_p,
                                                        &ntmp);
        stop();
        t_bcf_get_genotypes += report();

        start();
        uint32_t num_samples = bcf_hdr_nsamples(hdr);
        stop();
        t_bcf_hdr_nsamples += report();
        num_gts_per_sample /= num_samples;
        int32_t *gt_i = gt_p;

        start();
        for (j = 0; j < num_samples; ++j) {
            uint32_t gt = 0;
            for (k = 0; k < num_gts_per_sample; ++k) {
                gt += bcf_gt_allele(gt_i[k]);
            }

            packed_ints[int_i] += gt << (30 - 2*two_bit_i);
            two_bit_i += 1;
            if (two_bit_i == 16) {
                two_bit_i = 0;
                int_i += 1;
            }

            sum += gt;
            gt_i += num_gts_per_sample;
        }
        stop();
        t_pack += report();

        start();
        p.sum = sum;
        p.len = 0;
        int *j = (int *) malloc (sizeof(int));
        j[0] = i;
        priq_push(q, j, p);
        stop();
        t_q += report();


        start();
        fwrite(packed_ints, sizeof(uint32_t), num_ints,gt_of);
        stop();
        t_write += report();


        t_sum += sum;

        bcf_destroy(t_line);
        free(md);
//}}}
#endif
    }
    fclose(gt_of);
    fclose(md_of);


    md_of = fopen("md.tmp.packed","r");

    while ( priq_top(q, &p) != NULL ) {
        int *d = priq_pop(q, &p);


        uint32_t start = 0;
        if (*d != 0)
            start = md_index[*d - 1];

        uint32_t len = md_index[*d] - start;

        fseek(md_of, start*sizeof(char), SEEK_SET);
        char buf[len+1];
        fread(buf, sizeof(char), len, md_of);
        buf[len] = '\0';

        printf("%d\t%u\t%u\t%s\n", *d, start, len, buf);
    }

    fclose(md_of);


    printf("sum:%u\n\n", t_sum);

    /*
    double t = t_bcf_read +
               t_bcf_dup +
               t_bcf_unpack +
               t_bcf_get_genotypes +
               t_bcf_hdr_nsamples +
               t_q +
               t_get_md +
               t_md_write +
               t_pack;
    printf(
           "t_bcf_read:%lu %f\n"
           "t_bcf_dup:%lu %f\n"
           "t_bcf_unpack:%lu %f\n"
           "t_get_md:%lu %f\n"
           "t_md_write:%lu %f\n"
           "t_bcf_get_genotypes:%lu %f\n"
           "t_bcf_hdr_nsamples:%lu %f\n"
           "t_pack:%lu %f\n"
           "t_q:%lu %f\n"
           "t_write:%lu %f\n",
           t_bcf_read,
           t_bcf_read/t,
           t_bcf_dup,
           t_bcf_dup/t,
           t_bcf_unpack,
           t_bcf_unpack/t,
           t_get_md,
           t_get_md/t,
           t_md_write,
           t_md_write/t,
           t_bcf_get_genotypes,
           t_bcf_get_genotypes/t,
           t_bcf_hdr_nsamples,
           t_bcf_hdr_nsamples/t,
           t_pack,
           t_pack/t,
           t_q,
           t_q/t,
           t_write,
           t_write/t);
    */

    free(md_index);
    free(packed_ints);
#endif
}
