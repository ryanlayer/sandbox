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
	
	int *T = (int *) malloc(i * sizeof(int));
	for (j = 0; j < i; j++) {
		int lo = -1, hi = d;
		int row = log(j+1) / log(2);
		int num_prev = pow(2, row) - 1;
		int row_j = j - num_prev;
		int hi_v = 2*row_j + 1;
		int lo_v = pow(2, row+1) - (2*row_j + 1);
		int d_j = (hi_v*hi + lo_v*lo) / pow(2,row + 1);
		T[ j ] = D[ d_j ];
	}

	/*
	for (j = 0; j < i; j++) {
		int lo = -1, hi = d;
		int x = (j*hi + (i - j)*lo)/i;
		printf("%d\tI:%d\tT:%d\tD:%d\n", 
				j, I[j], T[j],
				D[j]);
	}
	*/


	unsigned long bs = 0;
	unsigned long is = 0;
	int w = 0;

	/* Search list */
	/*
	for (j = 0; j < q; j++) {

		int r = rand();
		start();
		int a = b_search(r, D, d, -1, d);
		stop();
		bs += report();

		start();
		int b = i_b_search(r, D, d, I, i);
		stop();
		is += report();

		if (a != b)
			++w;
	}

	printf("%lu\t%lu\t%f\n", bs, is, (((double)bs) / ((double)is)) );
	*/

	return 0;
}
