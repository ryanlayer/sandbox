#include <stdio.h>
#include <htslib/hts.h>
#include <htslib/vcf.h>
#include <htslib/kstring.h>
#include <strings.h>
#include "timer.h"
#include "pq.h"

int main(int argc, char **argv)
{

    if (argc < 3) {
        fprintf(stderr,"%s <bcf file> <num vars>\n", argv[0]);
        return 1;
    }


    char *fname = argv[1];
    uint32_t num_vars = atoi(argv[2]);

    htsFile *fp    = hts_open(fname,"rb");
    bcf_hdr_t *hdr = bcf_hdr_read(fp);
    bcf1_t *line    = bcf_init1();
    int32_t *gt_p = NULL;

    uint32_t num_records = bcf_hdr_nsamples(hdr);
    printf("num_records:%u\n", num_records);
    
    int32_t i, j, k, ntmp = 0, int_i = 0, two_bit_i = 0, sum, t_sum = 0;

    uint32_t num_ints = 1 + ((num_records - 1) / 16);

    pri_queue q = priq_new(0);
    priority p;

    fprintf(stderr, "num_records:%u\t num_ints:%u\n", num_records, num_ints);
    uint32_t *packed_ints = (uint32_t *) calloc(num_ints, sizeof(uint32_t));

    FILE *gt_of = fopen("gt.tmp.packed","wb");
    FILE *md_of = fopen("md.tmp.packed","w");

    uint32_t *md_index = (uint32_t *) malloc(num_vars * sizeof(uint32_t));
    uint32_t md_i = 0;

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
    for (i = 0; i < num_vars; ++i) {
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
        //fwrite(md, sizeof(char), len, md_of);
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
    }
    fclose(gt_of);
    fclose(md_of);


    md_of = fopen("md.tmp.packed","r");

    /*
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
    */

    fclose(md_of);


    printf("sum:%u\n\n", t_sum);

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

    free(md_index);
    free(packed_ints);
}
