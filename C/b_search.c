#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int b_search(int key, int *D, int D_size, int lo, int hi)
{
	//int lo = -1, hi = D_size, mid;
	int i = 0;
	int mid;
	while ( hi - lo > 1) {
		++i;
		mid = (hi + lo) / 2;
		//printf("%d(%d)\t", mid, D[mid]);
		if ( D[mid] < key )
			lo = mid;
		else
			hi = mid;
	}
	
	printf("%d\n", i);

	return hi;
}

int i_b_search(int key, int *D, int D_size, int *I, int I_size)
{
		int b = b_search(key, I, I_size, -1, I_size);

		int lo = -1, hi = D_size;
		int new_hi = ( (b+1)*hi + (I_size - (b+1))*lo ) / I_size;
		int new_lo = ( (b)*hi + (I_size - (b+1))*lo ) / I_size;

		if (b == 0)
			new_lo = -1;
		else if (b == I_size) {
			new_hi = D_size;
			new_lo = ( (b-1)*hi + (I_size - (b+1))*lo ) / I_size;
		}

		return b_search(key, D, D_size, new_lo, new_hi);
}

/*
void index(int i, int *I, int *D, int D_size)
{

}
*/

int compare_int(const void *a, const void *b)
{
	int *a_i = (int *)a;
	int *b_i = (int *)b;

	return *a_i - *b_i;
}


int main(int argc, char *argv[])
{

	if (argc < 4) {
		fprintf(stderr, "usage:\t%s <D size > <Q size> <I size> "
				"<seed>\n", argv[0]);
		return 1;
	}
	int d = atoi(argv[1]); // size of data set D
	int q = atoi(argv[2]); // size of query set Q
	int i = atoi(argv[3]); // size of index I
	int seed = atoi(argv[4]);

	srand(seed);

	int *D = (int *) malloc(d * sizeof(int));

	int j;
	for (j = 0; j < d; j++)
		D[j] = rand();

	qsort(D, d, sizeof(int), compare_int);

	/*  Print list
	for (j = 0; j < d; j++)
		printf("%d %d\n", j, D[j]);
	*/

	int *I = (int *) malloc(i * sizeof(int));

	for (j = 1; j < i; j++) {
		int lo = -1, hi = d;
		I[ j - 1 ] = D[ ( j*hi + (i - j)*lo ) / i];
	}

	unsigned long bs = 0;
	unsigned long is = 0;
	int w = 0;

	/* Search list */
	for (j = 0; j < q; j++) {

		/*
		printf("%d\t", r);
		int a = b_search(r, D, d, -1, d);
		int b = b_search(r, I, i, -1, i);

		int lo = -1, hi = d;
		int new_hi = ( (b+1)*hi + (i - (b+1))*lo ) / i;
		int new_lo = ( (b)*hi + (i - (b+1))*lo ) / i;

		if (b == 0)
			new_lo = -1;
		else if (b == i) {
			new_hi = d;
			new_lo = ( (b-1)*hi + (i - (b+1))*lo ) / i;
		}

		int c = b_search(r, D, d, new_lo, new_hi);

		printf("d:%d\ti:%d\t%d\n",a,b,c);
		*/

		printf("b:\n");
		int r = rand();
		start();
		int a = b_search(r, D, d, -1, d);
		stop();
		bs += report();

		printf("i:\n");
		start();
		int b = i_b_search(r, D, d, I, i);
		stop();
		is += report();

		if (a != b)
			++w;
	}

	printf("%lu\t%lu\t%f\n", bs, is, (((double)bs) / ((double)is)) );

	return 0;
}
