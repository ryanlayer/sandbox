#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "bsearch_lib.h"

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

//{{{int i_to_I(int i, int I_size, int D_size)
int i_to_I(int i, int I_size, int D_size)
{
	int lo = -1, hi = D_size;
	int regions = I_size + 1;

	return ( (i+1)*hi + (regions - (i+1))*lo) / (regions);
}
//}}}

//{{{int i_to_T(int i, int T_size, int D_size)
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
//}}}

//{{{void region_to_hi_lo(int region, int I_size, int D_size, int *D_hi, int
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
//}}}

//{{{int i_search(int key, int *I, int I_size, int D_size, int *D_hi, int *D_lo)
int i_search(int key, int *I, int I_size, int D_size, int *D_hi, int *D_lo)
{
	int region = b_search(key, I, I_size, -1, I_size);
	region_to_hi_lo(region, I_size, D_size, D_hi, D_lo);
	return region;
}
//}}}

//{{{int t_search(int key, int *T, int T_size, int D_size, int *D_hi, int
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
//}}}

//{{{int t_b_search(int key, int *D, int D_size, int *T, int T_size)
int t_b_search(int key, int *D, int D_size, int *T, int T_size)
{
	int hi, lo;
	int region = t_search(key, T, T_size, D_size, &hi, &lo);
	//printf("t:%d,%d,%d\n",region,lo,hi);
	int r = b_search(key, D, D_size, lo, hi);
	return r;
}
//}}}

//{{{int i_b_search(int key, int *D, int D_size, int *I, int I_size, int *r)
int i_b_search(int key, int *D, int D_size, int *I, int I_size)
{
	int hi, lo;
	int region = i_search(key, I, I_size + 1, D_size, &hi, &lo);
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
