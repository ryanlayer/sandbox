#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

struct interval
{
    uint32_t start,end;
};

struct interval bin_2_pos(int bin_id)
{
    struct interval i;
    i.start = 0;
    i.end = 0;

    if (bin_id == 0) {
        i.start = 1;
        i.end = pow(2,29);
    } else if (bin_id <= 8) {
        i.start = (bin_id - 1) * pow(2,26);
        i.end = i.start + pow(2,26);
    } else if (bin_id <= 72) {
        i.start = (bin_id - 8) * pow(2,23);
        i.end = i.start + pow(2,23);
    } else if (bin_id <= 584) {
        i.start = (bin_id - 72) * pow(2,20);
        i.end = i.start + pow(2,20);
    } else if (bin_id <= 4680) {
        i.start = (bin_id - 584) * pow(2,17);
        i.end = i.start + pow(2,17);
    } 

    return i;
}


int main(int argc, char **argv)
{

    if (argc != 2){
        fprintf(stderr, "usage:\t%s <bam file index>\n", argv[0]);
        return 1;
    }

    char *file_name = argv[1];
    int counter;
    FILE *ptr_myfile;

    ptr_myfile=fopen(file_name,"rb");
    if (!ptr_myfile)
    {
            printf("Unable to open file!");
            return 1;
    }

    char magic[4];
    fread(&magic,sizeof(char),4,ptr_myfile);
    printf("magic:%s\n",magic);

    int32_t n_ref;
    fread(&n_ref,sizeof(int32_t),1,ptr_myfile);
    printf("n_ref:%d\n",n_ref);

    int32_t nn;
    for(nn = 0; nn < n_ref; ++nn){

      int32_t n_bin;
      fread(&n_bin,sizeof(int32_t),1,ptr_myfile);
      printf(" n_bin:%lli\n",n_bin);
      
      int32_t i;
      for (i = 0; i < n_bin; ++i) {
	uint32_t bin;
	fread(&bin,sizeof(uint32_t),1,ptr_myfile);

	int32_t n_chunk;
	fread(&n_chunk,sizeof(int32_t),1,ptr_myfile);
	printf("  n_chunk:%lli\n",n_chunk);

	int32_t j;
	for (j = 0; j < n_chunk; ++j) {
	  uint64_t chunk_beg, chunk_end;
	
	  fread(&chunk_beg,sizeof(uint64_t),1,ptr_myfile);
	  fread(&chunk_end,sizeof(uint64_t),1,ptr_myfile);
	  
	  printf ( " offset %llu\n ", chunk_beg ) ;
	
	}
      }
      int32_t n_intv;
      fread(&n_intv,sizeof(int32_t),1,ptr_myfile);
      
      int32_t k; uint64_t ioffset;
      for(k = 0; k < n_intv; ++k){
	fread(&ioffset,sizeof(uint64_t),1,ptr_myfile);
      }
    }

	
    fclose(ptr_myfile);
    return 0;
}
