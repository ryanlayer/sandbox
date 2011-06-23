#ifndef __BSEARCH_LIB_H__
#define __BSEARCH_LIB_H__

#include "bsearch_lib.h"

int b_search(int key, int *D, int D_size, int lo, int hi);

int i_to_I(int i, int I_size, int D_size);

int i_to_T(int i, int T_size, int D_size);

void region_to_hi_lo(int region, int I_size, int D_size, int *D_hi, int *D_lo);

int i_search(int key, int *I, int I_size, int D_size, int *D_hi, int *D_lo);

int t_search(int key, int *T, int T_size, int D_size, int *D_hi, int *D_lo);

int t_b_search(int key, int *D, int D_size, int *T, int T_size);

int i_b_search(int key, int *D, int D_size, int *I, int I_size);

int compare_int(const void *a, const void *b);

#endif
