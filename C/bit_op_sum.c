#include <immintrin.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include "timer.h"

const int tab32[32] = {
    0,  9,  1, 10, 13, 21,  2, 29,
    11, 14, 16, 18, 22, 25,  3, 30,
    8, 12, 20, 28, 15, 17, 24,  7,
    19, 27, 23,  6, 26,  5,  4, 31};

int log2_32 (uint32_t value)
{
    value |= value >> 1;
    value |= value >> 2;
    value |= value >> 4;
    value |= value >> 8;
    value |= value >> 16;
    return tab32[(uint32_t)(value*0x07C4ACDD) >> 27];
}

int main(int argc, char** argv)
{
    if (argc != 4) {
        printf("usage:\t%s <N> <itterations> <bits set>\n", argv[0]);
        return 1;
    }

    unsigned int N = atoi(argv[1]);
    unsigned int L = atoi(argv[2]);
    unsigned int B = atoi(argv[3]);

    unsigned int *R1, *R2, *R3, *R4;

    //I = (unsigned int *) malloc ( N * sizeof(unsigned int));
    unsigned int *I = (unsigned int *) malloc ( N * sizeof(unsigned int));

    R1 = (unsigned int *) malloc ( N *32* sizeof(unsigned int));
    R2 = (unsigned int *) malloc ( N *32* sizeof(unsigned int));
    R3 = (unsigned int *) malloc ( N *32* sizeof(unsigned int));
    R4 = (unsigned int *) malloc ( N *32* sizeof(unsigned int));

    __declspec(align(64)) unsigned int *R5 = (unsigned int *) malloc ( N *32* sizeof(unsigned int));

    unsigned int i,j,l;
    float t1 = 0, t2 = 0, t3 = 0;
    for (i = 0; i < N; ++i) {
        I[i] = 0;
        for (j = 0; j < B; ++j) 
            I[i] += 1 << (rand()%31);
    }

    for (i = 0; i < N*32; ++i) {
        R1[i] = 0;
        R2[i] = 0;
        R3[i] = 0;
        R4[i] = 0;
        R5[i] = 0;
    }

    unsigned int v,t;
    for ( l=0; l<L; ++l) {
        start();
        for (i = 0; i < N-1; ++i) {
            v = I[i];
            for (j = 0; j < 32; ++j)
                R1[j+i*32] += (v >> (31-j)) & 1;
        }
        stop();
        t1+=report();
    }

    for ( l=0; l<L; ++l) {
        start();

        for (i = 0; i < N-1; ++i) {
            v = I[i];
            for (;v;) {
                t = (unsigned int)log2( v&(-v));
                v &= v - 1; 
                R2[i] += t;
            }
        }
        stop();
        t2+=report();
    }

    unsigned int i_shift;
    for ( l=0; l<L; ++l) {
        start();

        for (i = 0; i < N-1; ++i) {
            v = I[i];
            i_shift = i*32 + 31;
            for (;v;) {
                t = log2_32(v&(-v));
                v &= v - 1; 
                R3[i_shift-t] += 1;
            }
        }
        stop();
        t3+=report();
    }

    float t4 = 0;
    int ts[4], t_i = 0;
    for ( l=0; l<L; ++l) {
        start();
        for (i = 0; i < N-1; ++i) {
            v = I[i];
            i_shift = i*32 + 31;
            for (;v;) {
                v &= v - 1; 
                R4[i_shift-t] += v;
            }
        }
        stop();
        t4+=report();
    }


    float t5 = 0;
    for ( l=0; l<L; ++l) {
        start();
        __declspec(align(64)) int rshift_4[8] = { 31, 30, 29, 28, 27, 26, 25, 24 };
        __declspec(align(64)) int rshift_3[8] = { 23, 22, 21, 20, 19, 18, 17, 16 };
        __declspec(align(64)) int rshift_2[8] = { 15, 14, 13, 12, 11, 10, 9, 8 };
        __declspec(align(64)) int rshift_1[8] = { 7, 6, 5, 4, 3, 2, 1, 0 };

        __declspec(align(64)) int masks[8] =  { 1, 1, 1, 1, 1, 1, 1, 1 };

        __m256i *R5_avx = (__m256i *)R5;

        __m256i *rshift_1_avx = (__m256i *)rshift_1;
        __m256i *rshift_2_avx = (__m256i *)rshift_2;
        __m256i *rshift_3_avx = (__m256i *)rshift_3;
        __m256i *rshift_4_avx = (__m256i *)rshift_4;
        __m256i *masks_avx = (__m256i *)masks;

        __m256i s_1 = _mm256_load_si256(rshift_1_avx);
        __m256i s_2 = _mm256_load_si256(rshift_2_avx);
        __m256i s_3 = _mm256_load_si256(rshift_3_avx);
        __m256i s_4 = _mm256_load_si256(rshift_4_avx);

        __m256i m = _mm256_load_si256(masks_avx);
        __m256i y1, y2, y3;

        for (i = 0; i < N-1; ++i) {
            y1 = _mm256_set1_epi32(I[i]);

            y2 = _mm256_srlv_epi32 (y1, s_1);
            y3 = _mm256_and_si256 (y2, m);
            R5_avx[3+i*4] = _mm256_add_epi32 (R5_avx[3+i*4], y3);

            y2 = _mm256_srlv_epi32 (y1, s_2);
            y3 = _mm256_and_si256 (y2, m);
            R5_avx[2+i*4] = _mm256_add_epi32 (R5_avx[2+i*4], y3);

            y2 = _mm256_srlv_epi32 (y1, s_3);
            y3 = _mm256_and_si256 (y2, m);
            R5_avx[1+i*4] = _mm256_add_epi32 (R5_avx[1+i*4], y3);

            y2 = _mm256_srlv_epi32 (y1, s_4);
            y3 = _mm256_and_si256 (y2, m);
            R5_avx[0+i*4] = _mm256_add_epi32 (R5_avx[0+i*4], y3);
        }
        stop();
        t5+=report();
    }


    printf("%f\t%f\t%f\t%f\t%f\n", t1/L, t2/L, t3/L, t4/L, t5/L);

    for (i = 0; i < N*32; ++i) {
        if (R1[i] != R5[i])
            printf("%u\t%u\t%u\t%u\n",i,R1[i],R2[i],R5[i]);
    }

}
