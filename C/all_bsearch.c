#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "bsearch_lib.h"

int main(int argc, char *argv[])
{

	if (argc < 5) {
		fprintf(stderr, "usages:\t%s <D size > <Q size> <I size> "
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

	/* PRINT LIST
	for (j = 0; j < d; j++)
		printf("%d\t%d\n", j, D[j]);
	*/

	int *I = (int *) malloc(i * sizeof(int));


	for (j = 0; j < i; j++)
		I[ j ] = D[ i_to_I(j,i,d) ];
	
	int *T = (int *) malloc(i * sizeof(int));

	for (j = 0; j < i; j++) 
		T[ j ] = D[ i_to_T(j,i,d) ];


	/* PRINT TREE and INDEX
	for (j = 0; j < d; j++)
		printf("%d\ti:%d,%d\tt:%d,%d\n", 
				j,
				I[j],i_to_I(j,i,d),
				T[j],i_to_T(j,i,d)
			  );
	*/



	/* Search list */
	unsigned long bs_1 = 0;
	unsigned long is_1 = 0;
	unsigned long ts_1 = 0;
	unsigned long bs_2 = 0;
	unsigned long is_2 = 0;
	unsigned long ts_2 = 0;
	int w = 0;
	for (j = 0; j < q; j++) {

		int r = rand();

		start();
		int a = b_search(r, D, d, -1, d);
		stop();
		bs_1 += report();

		start();
		int R;
		int b = i_b_search(r, D, d, I, i, &R);
		stop();
		is_1 += report();


		start();
		int c = t_b_search(r, D, d, T, i);
		stop();
		ts_1 += report();

		start();
		a = b_search(r, D, d, -1, d);
		stop();
		bs_2 += report();

		start();
		b = i_b_search(r, D, d, I, i, &R);
		stop();
		is_2 += report();


		start();
		c = t_b_search(r, D, d, T, i);
		stop();
		ts_2 += report();


		if ( (a != b) || (a != c) ) {
			printf("%d\ta:%d\tb:%d\tc:%d\n", r,a,b,c);
			++w;
		}
	}

	printf("%lu,%lu\t%lu,%lu\t%lu,%lu\n",
			bs_1, bs_2,
			is_1, is_2,
			ts_1, ts_2);
	return 0;
}
