#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) 
{
	if (argc < 5) {
		printf("usage:\t window_sizes <window size> <slide size> "
				"<chrom size> <local mem>\n");
		return 0;
	}
	int window_size = atoi(argv[1]);
	int slide_size = atoi(argv[2]);
	int chrom_size = atoi(argv[3]);
	int local_mem = atoi(argv[4]);

	int local_size = local_mem / sizeof(int);

	printf("local_size:%d\n", local_size);

	int local_windows = (local_size - window_size)/slide_size;

	printf("local_windows:%d\n", local_windows);

	int useable_local_windows = (local_size - 2*window_size)/slide_size;

	printf("useable_local_windows:%d\n", useable_local_windows);
	
	return 1;
}
