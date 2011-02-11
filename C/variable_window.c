#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>


void extract_dense(int *A, int data_length, int window_length, 
		int slide_length, int bank_size, int **D) 
{
	
	int j, windows =  (data_length - window_length) / slide_length;
	*D = (int *) calloc(windows, sizeof(int));
	for (j = 0; j < (data_length - window_length); j += slide_length) {
		int k, window_sum = 0;
		for (k = j; k < j + window_length; k++) {
			window_sum += A[k];
			//printf("A[%d]:%d window_sum:%d\n", k, A[k], window_sum);
		}
		(*D)[j/slide_length] = window_sum;
		//printf("D[%d]:%d\n", j/slide_length, window_sum);
	}
}



int main(int argc, char *argv[]) {

	if (argc < 5) {
		printf("usage: variable_window <array size> <window size> "
				"<slide size> <bank size>\n");
		return 0;
	}

	int N = atoi(argv[1]), 
			W = atoi(argv[2]),
			S = atoi(argv[3]),
			M = atoi(argv[4]),
			*A = (int *) malloc(N * sizeof(int)),
			i;

	for (i = 0; i < N; i++) {
		A[i] = rand() % 10;
		//printf("%d\n", A[i]);
	}

	int *dense;
	extract_dense(A, N, W, S, M, &dense);

	for (i = 0; i < (N-W)/S; i++)
		printf("%d\n",dense[i]);

	int *dense_1;

	//have the window slide over a region
	for (i = 0; i < ; i +=M) {	
		//what is the max amount of slides I can get in space M?
		int max_wins = (M-W)/S;
		int trash_wins = (W-S)/S;
		
	}

}

