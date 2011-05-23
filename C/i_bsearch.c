#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int i_bsearch(int key, int *D, int D_size)
{
	int lo = -1, hi = D_size, mid;
	while ( hi - lo > 1) {
		mid = (hi + lo) / 2;
		printf("%d,%d,%d\t", lo, mid, hi);
		if ( D[mid] < key )
			lo = mid;
		else
			hi = mid;
	}
	
	printf("\n");

	return hi;
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

	for (j = 0; j < d; j++)
		printf("%d %d\n", j, D[j]);


	for (j = 1; j < i; j++) {
		int lo = -1, hi = d;
		printf("%d %d\n",j, ( j*hi + (i - j)*lo ) / i);
	}


	for (j = 0; j < q; j++) {
		int r = rand();
		printf("%d %d\n", r, i_bsearch(r, D, d));
	}


	return 0;
}
