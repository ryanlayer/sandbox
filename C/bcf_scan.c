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
    uint32_t num = atoi(argv[2]);
    int64_t offset = atoll(argv[3]);

    htsFile *fp = hts_open(in_file_name,"rb");
    if ( !fp )
        err(EX_DATAERR, "Could not read file: %s", in_file_name);

    BGZF *gzfp = hts_get_bgzfp(fp);

    bcf_hdr_t *hdr = bcf_hdr_read(fp);
    if ( !hdr )
        err(EX_DATAERR, "Could not read the header: %s", in_file_name);

    bcf1_t *line = bcf_init1();
    kstring_t str;
    str.s = 0; str.l = str.m = 0;

    int ret;

    if (offset != -1)
        bgzf_seek(gzfp, offset, SEEK_SET);

    fprintf(stdout,
            "bgzf_tell:%lld\tblock_address:%lld\tblock_offset:%d\n",
            bgzf_tell(gzfp),
            gzfp->block_address,
            gzfp->block_offset);

    uint32_t i;
    for ( i  = 0; i < num; ++ i) {
        ret = bcf_read(fp, hdr, line);
        vcf_format1(hdr, line, &str);
        fprintf(stdout, "%s", str.s);
        fprintf(stdout,
                "bgzf_tell:%lld\tblock_address:%lld\tblock_offset:%d\n",
                bgzf_tell(gzfp),
                gzfp->block_address,
                gzfp->block_offset);
        str.l = 0;
    }
}
