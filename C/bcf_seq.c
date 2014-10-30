#include <stdio.h>
#include <htslib/hts.h>
#include <htslib/vcf.h>
#include <htslib/kstring.h>
#include <strings.h>
#include "timer.h"
#include "pq.h"

int nlz1(unsigned x)
{
    int n;

    if (x == 0) return(32);
    n = 0;
    if (x <= 0x0000FFFF) {n = n +16; x = x <<16;}
    if (x <= 0x00FFFFFF) {n = n + 8; x = x << 8;}
    if (x <= 0x0FFFFFFF) {n = n + 4; x = x << 4;}
    if (x <= 0x3FFFFFFF) {n = n + 2; x = x << 2;}
    if (x <= 0x7FFFFFFF) {n = n + 1;}
    return n;
}

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

    uint32_t num_inds = bcf_hdr_nsamples(hdr);
    
    int32_t i, j, k, ntmp = 0, int_i = 0, two_bit_i = 0, sum, t_sum = 0;

    uint32_t num_ind_ints = 1 + ((num_inds - 1) / 16);

    pri_queue q = priq_new(0);
    priority p;

    uint32_t *packed_ints = (uint32_t *) calloc(num_ind_ints,
                                                sizeof(uint32_t));

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
        two_bit_i = 0;

        int r = bcf_read(fp, hdr, line);
        
        // Copy
        bcf1_t *t_line = bcf_dup(line);

        // Unpack
        bcf_unpack(t_line, BCF_UN_ALL);

        // Get metadata
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

        // Write metadata
        md_i += strlen(md);
        md_index[i] = md_i;
        fprintf(md_of, "%s", md);

        // Get gentotypes
        uint32_t num_gts_per_sample = bcf_get_genotypes(hdr,
                                                        t_line,
                                                        &gt_p,
                                                        &ntmp);
        num_gts_per_sample /= num_inds;
        int32_t *gt_i = gt_p;
        
        // Pack genotypes
        for (j = 0; j < num_inds; ++j) {
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

        // Get a priority for the variant based on the sum and number of 
        // leading zeros
        p.sum = sum;
        uint32_t prefix_len = 0;
        j = 0;
        while ((j < num_ind_ints) && (packed_ints[j] == 0)){
            prefix_len += 32;
            j += 1;
        }
        if (j < num_ind_ints)
            prefix_len += nlz1(packed_ints[j]);
        
        // Push it into the q
        p.len = prefix_len;
        int *j = (int *) malloc (sizeof(int));
        j[0] = i;
        priq_push(q, j, p);

        // Write to file
        fwrite(packed_ints, sizeof(uint32_t), num_ind_ints,gt_of);

        memset(packed_ints, 0, num_ind_ints*sizeof(uint32_t));

        t_sum += sum;

        bcf_destroy(t_line);
        free(md);
    }
    fclose(gt_of);
    fclose(md_of);


    md_of = fopen("md.tmp.packed","r");
    FILE *md_out = fopen("md.bim","w");
    gt_of = fopen("gt.tmp.packed","rb");
    FILE *s_gt_of = fopen("s.gt.tmp.packed","wb");

    // Get variants in order and rewrite a variant-major sorted matrix
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

        fseek(gt_of, (*d)*num_ind_ints*sizeof(uint32_t), SEEK_SET);
        fread(packed_ints, sizeof(uint32_t), num_ind_ints, gt_of);
        fwrite(packed_ints, sizeof(uint32_t), num_ind_ints,s_gt_of);

        fprintf(md_out, "%s\n", buf);
    }

    fclose(md_out);
    fclose(md_of);
    fclose(gt_of);
    fclose(s_gt_of);


    /*
     * In a packed-int variant-major matrix there will be a num_vars
     * number of rows, and a num_inds number of values packed into
     * num_inds_ints number of intergers.  For examples, 16 rows of 16 values
     * will be 16 ints, where each int encodes 16 values.
     *
     */
    
    uint32_t num_var_ints = 1 + ((num_vars - 1) / 16);

    uint32_t *I_data = (uint32_t *)calloc(num_var_ints*16,sizeof(uint32_t));
    uint32_t **I = (uint32_t **)malloc(16*sizeof(uint32_t*));
    for (i = 0; i < 16; ++i)
        I[i] = I_data + i*num_var_ints;
    uint32_t I_i = 0, I_int_i = 0;

    uint32_t v;

    s_gt_of = fopen("s.gt.tmp.packed","rb");
    FILE *rs_gt_of = fopen("r.s.gt.tmp.packed","wb");

    // Write these to values to that this is a well-formed uncompressed 
    // packed int binary file (ubin) file
    fwrite(&num_vars, sizeof(uint32_t), 1, rs_gt_of);
    fwrite(&num_inds, sizeof(uint32_t), 1, rs_gt_of);
     
    /* 
     * we need to loop over the columns in the v-major file.
     * There are num_vars rows, and num_ind_ints 16-ind packed columns
     *
     * In this loop :
     *  i: cols in var-major form, rows in ind-major form
     *  j: rows in var-major form, cols in ind-major form
     */
    uint32_t num_inds_to_write = num_inds;
    for (i = 0; i < num_ind_ints; ++i) { // loop over each int col
        for (j = 0; j < num_vars; ++j) { // loop over head row in that col
            // skip to the value at the row/col
            fseek(s_gt_of, 
                  j*num_ind_ints*sizeof(uint32_t) + //row
                  i*sizeof(uint32_t), //col
                  SEEK_SET);

            fread(&v, sizeof(uint32_t), 1, s_gt_of);

            // one int corresponds to a col of 16 two-bit values
            // two_bit_i will move across the cols
            for (two_bit_i = 0; two_bit_i < 16; ++two_bit_i) {
                I[two_bit_i][I_i] += ((v >> (30 - 2*two_bit_i)) & 3) << 
                                     (30 - 2*I_int_i);
            }
            I_int_i += 1;

            if (I_int_i == 16) {
                I_i += 1;
                I_int_i = 0;
            }
        }

        // When we are at the end of the file, and the number of lines 
        // is not a factor of 16, only write out the lines that contain values
        if (num_inds_to_write >= 16) {
            fwrite(I_data,
                   sizeof(uint32_t),
                   num_var_ints*16,
                   rs_gt_of);
            num_inds_to_write -= 16;
        } else {
            fwrite(I_data,
                   sizeof(uint32_t),
                   num_var_ints*num_inds_to_write,
                   rs_gt_of);
        }
        memset(I_data, 0, num_var_ints*16*sizeof(uint32_t));
        I_int_i = 0;
        I_i = 0;
    }

    fclose(s_gt_of);
    fclose(rs_gt_of);

    free(md_index);
    free(packed_ints);
}
