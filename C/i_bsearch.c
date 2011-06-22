#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//{{{int b_search(int key, int *D, int D_size, int lo, int hi)
int b_search(int key, int *D, int D_size, int lo, int hi)
{
	//int lo = -1, hi = D_size, mid;
	int i = 0;
	int mid;
	//fprintf(stderr, "k:%d\t", key);
	while ( hi - lo > 1) {
		++i;
		mid = (hi + lo) / 2;
		//printf("%d\t", D[mid]);
		//fprintf(stderr, "%d,%d,%d\t", lo, mid, hi, D[mid]);
		if ( D[mid] < key )
			lo = mid;
		else
			hi = mid;
	}
	//fprintf(stderr, "\n");
	
	return hi;
}
//}}}

int i_to_I(int i, int I_size, int D_size)
{
	int lo = -1, hi = D_size;
	int regions = I_size + 1;

	return ( (i+1)*hi + (regions - (i+1))*lo) / (regions);
}

int i_to_T(int i, int T_size, int D_size)
{
	int lo = -1, hi = D_size;
	double row_d = log(i + 1) / log(2);
	int row = (int) (row_d);
	//printf("%f\t%f\t%f\n", log(i+1),log(2),log(i + 1) / log(2));
	int prev = pow(2, row) - 1;
	int i_row = i - prev;

	int hi_v = 2*i_row + 1;
	int lo_v = pow(2, row + 1) - (2*i_row +1);
	int div = pow(2,row + 1);
	int r = ( hi_v*hi + lo_v*lo) / div;

	//printf("hi:%d\tlo:%d\trow:%d\thi_v:%d\tlo_v:%d\tdiv:%d\tr:%d\n",
			//hi, lo, row, hi_v, lo_v, div, r);

	return r;
}

void region_to_hi_lo(int region, int I_size, int D_size, int *D_hi, int *D_lo)
{
	int lo = -1, hi = D_size;
	int new_hi = ( (region+1)*hi + (I_size - (region+1))*lo ) / I_size;
	int new_lo = (( (region)*hi + (I_size - (region+1))*lo ) / I_size) - 1;

	//--new_lo;

	if (region == 0)
		new_lo = -1;
	else if (region == I_size) {
		new_hi = D_size;
		new_lo = ( (region-1)*hi + (I_size - (region+1))*lo ) / I_size;
		//--new_lo;
	}
	

	*D_hi = new_hi;
	*D_lo = new_lo;
}

int i_search(int key, int *I, int I_size, int D_size, int *D_hi, int *D_lo)
{
	int region = b_search(key, I, I_size, -1, I_size);
	region_to_hi_lo(region, I_size, D_size, D_hi, D_lo);
	return region;
}

int t_search(int key, int *T, int T_size, int D_size, int *D_hi, int *D_lo)
{
	int b = 0;

	//printf("t:");
	while (b < T_size) {
		//printf("%d,%d\t", T[b],b);
		if (key < T[b])
			b = 2*(b) + 1;
		else if (key > T[b])
			b = 2*(b) + 2;
		else
			break;
	}
	//printf("%d\n",b);

	int region = b - T_size;
	//printf("%d\t%d\n", b, region);
	region_to_hi_lo(b - T_size, T_size + 1, D_size, D_hi, D_lo);
	return region;

}

int t_b_search(int key, int *D, int D_size, int *T, int T_size)
{
	int hi, lo;
	int region = t_search(key, T, T_size, D_size, &hi, &lo);
	//printf("t:%d,%d,%d\n",region,lo,hi);
	int r = b_search(key, D, D_size, lo, hi);
	return r;
}


int i_b_search(int key, int *D, int D_size, int *I, int I_size, int *r)
{
	int hi, lo;
	int region = i_search(key, I, I_size + 1, D_size, &hi, &lo);
	*r = region;
	//printf("i:%d,%d,%d\n",region,lo,hi);
	return b_search(key, D, D_size, lo, hi);
}
//}}}

//{{{int compare_int(const void *a, const void *b)
int compare_int(const void *a, const void *b)
{
	int *a_i = (int *)a;
	int *b_i = (int *)b;

	return *a_i - *b_i;
}
//}}}


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
