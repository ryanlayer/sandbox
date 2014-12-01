#include <stdio.h>
#include <stdlib.h>
#include <zlib.h>
#include <string.h>
#include <sys/stat.h>
#include <assert.h>

//#define CHUNK 16384
//#define CHUNK 100

#define MAX(X,Y) ((X) > (Y) ? (X) : (Y))
#define MIN(X,Y) ((X) < (Y) ? (X) : (Y))


int main(int argc, char **argv)
{

    if (argc != 5) {
        fprintf(stderr, "usage:\t%s"
                        "<file name 1> <file name 2> <out file> <buf len>\n", argv[0]);
        return 1;
    }

    char *file_name_1 = argv[1];
    char *file_name_2 = argv[2];
    char *out_file_name = argv[3];
    uint32_t CHUNK = atoi(argv[4]);

    struct stat f1_stat;

    stat(file_name_1, &f1_stat);
    size_t f1_size = f1_stat.st_size;
    char *f1_buf = (char *)malloc((f1_size + 1)*sizeof(char));
    memset(f1_buf, 0, f1_size + 1);

    FILE *fp = fopen(file_name_1, "r");

    FILE *fp_o = fopen(out_file_name, "w");

    uint32_t c_size = 0, u_size = f1_size, h_size = f1_size;

    fwrite(&c_size, sizeof(uint32_t), 1, fp_o);
    fwrite(&u_size, sizeof(uint32_t), 1, fp_o);
    fwrite(&h_size, sizeof(uint32_t), 1, fp_o);

    fread(f1_buf, 1, f1_size, fp);

    fclose(fp);

    unsigned char in_buf[CHUNK];
    unsigned char out_buf[CHUNK*2];


    uint32_t f1_len = strlen(f1_buf);
    uint32_t in_buf_len = CHUNK;
    uint32_t f1_i = 0, in_buf_i = 0;

    z_stream strm;
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;

    uint32_t have;
    int flush;
    int ret = deflateInit(&strm, 6);
    if (ret != Z_OK) {
        fprintf(stderr, "Error setting up z stream\n");
        exit(1);
    }

    while (f1_i < f1_len) {
        // Move either the size of the buffer, or the remaining amout of f1 
        // to the buffer
        uint32_t mv_len = MIN(f1_len - f1_i, in_buf_len - in_buf_i);
        memcpy(in_buf + in_buf_i, f1_buf + f1_i, mv_len);

        // Move the head pointer of both the buffer and f1
        in_buf_i += mv_len;
        f1_i += mv_len;

        // When the buffer is full, compress it to out_buf and write it to a file
        if (in_buf_i == in_buf_len) {
            strm.avail_in = CHUNK;
            strm.next_in = in_buf;

            strm.avail_out = CHUNK * 2;
            strm.next_out = out_buf;

            /*
            fprintf(stderr,"before: "
                           "strm.avail_in:%u strm.next_in:%p in_buf:%p\t"
                           "strm.avail_out:%u strm.next_out:%p out_buf:%p"
                           "\n", 
                           strm.avail_in, 
                           strm.next_in, 
                           in_buf, 
                           strm.avail_out,
                           strm.next_out,
                           out_buf);
            */

            ret = deflate(&strm, Z_FULL_FLUSH);

            /*
            fprintf(stderr,"after: "
                           "strm.avail_in:%u strm.next_in:%p in_buf:%p\t"
                           "strm.avail_out:%u strm.next_out:%p out_buf:%p"
                           "\n", 
                           strm.avail_in, 
                           strm.next_in, 
                           in_buf, 
                           strm.avail_out,
                           strm.next_out,
                           out_buf);
            */

            if (ret == Z_BUF_ERROR)
                fprintf(stderr, 
                        "1 No progress is possible; either avail_in or avail_out was "
                        "zero %u\t%u.\n", strm.avail_in, strm.avail_out);
            else if (ret == Z_MEM_ERROR)
                fprintf(stderr, "Insufficient memory.\n");
            else if (ret == Z_STREAM_ERROR)
                fprintf(stderr, 
                        "The state (as represented in stream) is inconsistent, or "
                        "stream was NULL.");

 
            have = (CHUNK*2) - strm.avail_out;

            c_size += have;
            if (fwrite(out_buf, 1, have, fp_o) != have) {
                fprintf(stderr, "Error writing compressed value 0.\n");
                exit(1);
            } 
            //fwrite(in_buf, 1, CHUNK, fp_o);
            in_buf_i = 0;
        }
    }

    fp = fopen(file_name_2, "r");

    uint32_t read = 0, to_read = in_buf_len - in_buf_i;

    //printf("to_read:%d\n", to_read);


    while ( (read = fread(in_buf + in_buf_i, sizeof(char), to_read, fp)) == to_read ) {
        //fwrite(in_buf, 1, CHUNK, fp_o);
        strm.next_in = in_buf;
        strm.avail_in = CHUNK;
        strm.next_out = out_buf;
        strm.avail_out = CHUNK * 2;
        ret = deflate(&strm, Z_FULL_FLUSH);

        if (ret == Z_BUF_ERROR)
            fprintf(stderr, 
                    "2 No progress is possible; either avail_in or avail_out was "
                    "zero.\n");
        else if (ret == Z_MEM_ERROR)
            fprintf(stderr, "Insufficient memory.\n");
        else if (ret == Z_STREAM_ERROR)
            fprintf(stderr, 
                    "The state (as represented in stream) is inconsistent, or "
                    "stream was NULL.");

        assert(ret != Z_STREAM_ERROR);
        have = (CHUNK*2) - strm.avail_out;
        c_size += have;
        if (fwrite(out_buf, 1, have, fp_o) != have) {
            fprintf(stderr, "Error writing compressed value 1.\n");
            exit(1);
        } 
        
        u_size += CHUNK;
        in_buf_i = 0;
        to_read = CHUNK;
    }


    if (read > 0) {
        u_size += read;

        //fwrite(in_buf, 1, in_buf_i + read, fp_o);
        strm.next_in = in_buf;
        strm.avail_in = in_buf_i + read;
        strm.next_out = out_buf;
        strm.avail_out = CHUNK * 2;
        ret = deflate(&strm, Z_FULL_FLUSH);

        if (ret == Z_BUF_ERROR)
            fprintf(stderr, 
                    "3 No progress is possible; either avail_in or avail_out was "
                    "zero.\n");
        else if (ret == Z_MEM_ERROR)
            fprintf(stderr, "Insufficient memory.\n");
        else if (ret == Z_STREAM_ERROR)
            fprintf(stderr, 
                    "The state (as represented in stream) is inconsistent, or "
                    "stream was NULL.");

 
        assert(ret != Z_STREAM_ERROR);
        have = (CHUNK*2) - strm.avail_out;
        c_size += have;
        if (fwrite(out_buf, 1, have, fp_o) != have) {
            fprintf(stderr, "Error writing compressed value 2.\n");
            exit(1);
        } 
    }

    //fprintf(stderr,"%u\n", c_size);

    fseek(fp_o, 0, SEEK_SET);
    fwrite(&c_size, sizeof(uint32_t), 1, fp_o);
    fwrite(&u_size, sizeof(uint32_t), 1, fp_o);
    fclose(fp_o);


    // Deflate

  /* allocate inflate state */
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.avail_in = 0;
    strm.next_in = Z_NULL;
    ret = inflateInit(&strm);
    if (ret != Z_OK)
        return ret;

    fp_o = fopen(out_file_name, "rb");
    fread(&c_size, sizeof(uint32_t), 1, fp_o);
    fread(&u_size, sizeof(uint32_t), 1, fp_o);
    fread(&h_size, sizeof(uint32_t), 1, fp_o);

    char *final_out_buf = (char *) malloc(u_size);

    strm.avail_out = u_size;
    strm.next_out = (Bytef *)final_out_buf;

    /* decompress until deflate stream ends or end of file */
    do {
        strm.avail_in = fread(in_buf, 1, CHUNK, fp_o);
        if (ferror(fp_o)) {
            (void)inflateEnd(&strm);
            return Z_ERRNO;
        }
        if (strm.avail_in == 0)
            break;
        strm.next_in = in_buf;

        /* run inflate() on input until output buffer not full */
        do {
            //strm.avail_out = CHUNK;
            //strm.next_out = out_buf;
            ret = inflate(&strm, Z_NO_FLUSH);
            assert(ret != Z_STREAM_ERROR);  /* state not clobbered */
            switch (ret) {
            case Z_NEED_DICT:
                ret = Z_DATA_ERROR;     /* and fall through */
            case Z_DATA_ERROR:
            case Z_MEM_ERROR:
                (void)inflateEnd(&strm);
                return ret;
            }
            //have = CHUNK - strm.avail_out;
            //out_buf[have] = '\0';
            //printf("%s", out_buf);
            /*
            if (fwrite(out, 1, have, dest) != have || ferror(dest)) {
                (void)inflateEnd(&strm);
                return Z_ERRNO;
            }
            */
        } while (strm.avail_out == 0);

        /* done when inflate() says it's done */
    } while (ret != Z_STREAM_END);

    /* clean up and return */
    (void)inflateEnd(&strm);

    printf("%s", final_out_buf);

    return 0;
}
