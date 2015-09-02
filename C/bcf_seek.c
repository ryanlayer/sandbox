#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <sysexits.h>
#include <err.h>

#include <htslib/vcf.h>
#include <htslib/bgzf.h>
#include <htslib/kseq.h>
#include <htslib/hts.h>
#include <htslib/tbx.h>
#include <htslib/hfile.h>

int main(int argc, char **argv)
{
    char *in_file_name = argv[1];

    int64_t offset = atoll(argv[2]);

    uint32_t num = atoi(argv[3]);

    htsFile *fp = hts_open(in_file_name,"rb");
    if ( !fp )
        err(EX_DATAERR, "Could not read file: %s", in_file_name);

    BGZF *gzfp = hts_get_bgzfp(fp);

    fprintf(stdout, "after open: %llu\n", bgzf_tell(gzfp));

    bcf_hdr_t *hdr = bcf_hdr_read(fp);
    if ( !hdr )
        err(EX_DATAERR, "Could not read the header: %s", in_file_name);

    fprintf(stdout, "after header: %llu\n", bgzf_tell(gzfp));

    if (offset > 0 )
        if (bgzf_seek(gzfp, offset, SEEK_SET) != 0)
            err(EX_IOERR, "Error in seek\n");

    fprintf(stdout, "after seek: %lld\n", bgzf_tell(gzfp));


    uint32_t i = 0;

    bcf1_t *line = bcf_init1();
    kstring_t md = {0, 0, 0};

    if (bcf_hdr_set_samples(hdr, NULL, 0) != 0)
        err(EX_IOERR, "Error in bcf_hdr_set_samples\n");

    for ( i  = 0; i < num; ++ i) {
        uint64_t pos =  bgzf_tell(gzfp);
        if (bcf_read(fp, hdr, line) != 0)
            err(EX_IOERR, "Error in bcf_read\n");
        vcf_format1(hdr, line, &md);
        fprintf(stdout,"%lld\t%s", pos, md.s);
        md.l = 0;
    }
}
