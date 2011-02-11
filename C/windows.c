#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {

	if (argc < 3) {
		printf("usage: windows <N> <W> <S>\n");
		return 0;
	}
				

	int N = atoi(argv[1]);
	int w = atoi(argv[2]);
	int s = atoi(argv[3]);

	int i;
	for (i = 0; i < (N-w); i += s) {
		int j;
		for (j = i; j < i + w; j++) {
			printf("%d ", j);
			/*
			printf("%d-%d ", j, (j+1-w)%s);
			if (((j + 1) >= w) && (((j + 1 - w) % s) == 0)) 
				printf("end ");
			if (((j + 1 - w) % s) == 1) 
				printf("star ");
			*/
		}
		printf("\n");
	}

	return 1;
}
