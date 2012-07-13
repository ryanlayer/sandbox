#include <math.h>
#include <stdlib.h>
#include <time.h>
#include "mt.h"

#define min(a,b) \
	({ __typeof__ (a) _a = (a); \
	   __typeof__ (b) _b = (b); \
		_a < _b ? _a : _b; })

unsigned int HG19_size[] = {
 249250621, // chr1
 243199373, // chr1
 198022430, // chr1
 191154276, // chr1
 180915260, // chr1
 171115067, // chr1
 159138663, // chr1
 146364022, // chr1
 141213431, // chr1
 135534747, // chr1
 135006516, // chr1
 133851895, // chr1
 115169878, // chr1
 107349540, // chr1
 102531392, // chr1
 90354753 , // chr1
 81195210 , // chr1
 78077248 , // chr1
 59128983 , // chr1
 63025520 , // chr1
 48129895 , // chr1
 51304566 , // chr1
 155270560, // chr1
 59373566  // chr1
};

double HG19_percent[] = {
	0.080515696,
	0.159076651,
	0.223044049,
	0.28479282 ,
	0.343234071,
	0.398509555,
	0.449916288,
	0.497196415,
	0.542812742,
	0.586594676,
	0.630205976,
	0.673444298,
	0.710647747,
	0.745324984,
	0.77844581 ,
	0.807633203,
	0.833861779,
	0.859083156,
	0.878183655,
	0.898542857,
	0.914090308,
	0.930663277,
	0.980820493,
	1
};

static int compare_doubles (const void *_a, const void *_b)
{
	double *a = (double *) _a;
	double *b = (double *) _b;
	double temp = *a - *b;
	if (temp > 0)
		return 1;
	else if (temp < 0)
		return -1;
	else
		return 0;
}

void rand_human_chr(char *c, unsigned int *c_id) {
	int max = 23;
	//unsigned int bits = (int)( ceil(log(max_offset)/log(2) ) );
	//unsigned int mask = (2 << (bits-1)) - 1;
	unsigned int bits = 5;
	unsigned int mask = 31;

	double c_d = genrand_real1();
	//*c_id = get_rand(max,mask);
	unsigned int i = 0;
	while (c_d > HG19_percent[i])
		++i;

	*c_id = i;

	if ( (*c_id) == 23 ) 
		sprintf(c, "chrY"); 
	else if ( (*c_id) == 22) 
		sprintf(c, "chrX"); 
	else 
		sprintf(c, "chr%d", *c_id + 1); 
}

void rand_human_interval(unsigned int len,
						 char *chr,
						 unsigned int *start,
						 unsigned int *end)
{
	unsigned int c_id;
	rand_human_chr(chr, &c_id);

	unsigned int max_offset = HG19_size[c_id];

	unsigned int bits = (int)( ceil(log(max_offset)/log(2) ) );
	unsigned int mask = (2 << (bits-1)) - 1;

	*start = get_rand(max_offset,mask);
	*end = min(max_offset, *start+len+1);
}


int main(int argc, char **argv) {

	if (argc < 2) {
		fprintf(stderr, "%s N\n", argv[0]);
		return 1;
	}

	int n = atoi(argv[1]);

	init_genrand((unsigned) time(NULL));
	int i;
	for (i=0; i<n; i++) {
		char c[6];
		unsigned int start, end;
		rand_human_interval(100, c, &start, &end);
		printf("%s\t%u\t%u\n", c, start, end);
	}
}
