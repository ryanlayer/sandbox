#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "bsearch_lib.h"

int main(int argc, char *argv[])
{

	if (argc < 4) {
		fprintf(stderr, "usages:\t%s <D size > <Q size> "
				"<seed>\n", argv[0]);
		return 1;
	}
	int D_size = atoi(argv[1]); // size of data set D
	int Q_size = atoi(argv[2]); // size of query set Q
	int seed = atoi(argv[3]);

	srand(seed);

	int *D = (int *) malloc(D_size * sizeof(int));

	int j;
	for (j = 0; j < D_size; j++)
		D[j] = rand();

	qsort(D, D_size, sizeof(int), compare_int);

	/* PRINT LIST
	for (j = 0; j < d; j++)
		printf("%d\t%d\n", j, D[j]);
	*/

	/* Search list */
	unsigned long total_time = 0;
	unsigned long *times = (unsigned long *) malloc(
			Q_size * sizeof(unsigned long));
	for (j = 0; j < Q_size; j++) {
		int r = rand();

		start();
		int c = b_search(r, D, D_size, -1, D_size);
		stop();
		total_time += report();
		times[j] = report();
	}

	for (j = 0; j < Q_size; j++)
		printf("%lu\n", times[j]);

	return 0;
}
